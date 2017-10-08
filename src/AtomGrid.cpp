#include <cmath>
#include <limits>

#include "../api/numgrid.h"
#include "AtomGrid.h"
#include "becke_partitioning.h"
#include "bragg.h"
#include "error_handling.h"
#include "grid_radial.h"
#include "lebedev/sphere_lebedev_rule.h"
#include "parameters.h"

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)

int lebedev_table[33] = {6,    14,   26,   38,   50,   74,   86,   110,
                         146,  170,  194,  230,  266,  302,  350,  434,
                         590,  770,  974,  1202, 1454, 1730, 2030, 2354,
                         2702, 3074, 3470, 3890, 4334, 4802, 5294, 5810};

int get_closest_num_angular(int n)
{
    int m;

    for (int i = 0; i < MAX_ANGULAR_ORDER; i++)
    {
        m = lebedev_table[i];
        if (m >= n)
            return m;
    }

    NUMGRID_ERROR("Input n too high in get_closest_num_angular");
}

int get_angular_order(int n)
{
    for (int i = 0; i < MAX_ANGULAR_ORDER; i++)
    {
        if (lebedev_table[i] == n)
            return i;
    }

    NUMGRID_ERROR("No match found in get_angular_offset");
}

context_t *numgrid_new_atom_grid(const double radial_precision,
                                 const int min_num_angular_points,
                                 const int max_num_angular_points,
                                 const int proton_charge,
                                 const double alpha_max,
                                 const int max_l_quantum_number,
                                 const double alpha_min[])
{
    return AS_TYPE(context_t,
                   new AtomGrid(radial_precision,
                                min_num_angular_points,
                                max_num_angular_points,
                                proton_charge,
                                alpha_max,
                                max_l_quantum_number,
                                alpha_min));
}
AtomGrid::AtomGrid(const double radial_precision,
                   const int min_num_angular_points,
                   const int max_num_angular_points,
                   const int proton_charge,
                   const double alpha_max,
                   const int max_l_quantum_number,
                   const double alpha_min[])
{
    int min_num_angular_points_closest =
        get_closest_num_angular(min_num_angular_points);
    int max_num_angular_points_closest =
        get_closest_num_angular(max_num_angular_points);

    double *angular_x = new double[MAX_ANGULAR_ORDER * MAX_ANGULAR_GRID];
    double *angular_y = new double[MAX_ANGULAR_ORDER * MAX_ANGULAR_GRID];
    double *angular_z = new double[MAX_ANGULAR_ORDER * MAX_ANGULAR_GRID];
    double *angular_w = new double[MAX_ANGULAR_ORDER * MAX_ANGULAR_GRID];

    for (int i = get_angular_order(min_num_angular_points_closest);
         i <= get_angular_order(max_num_angular_points_closest);
         i++)
    {
        int angular_off = i * MAX_ANGULAR_GRID;
        ld_by_order(lebedev_table[i],
                    &angular_x[angular_off],
                    &angular_y[angular_off],
                    &angular_z[angular_off],
                    &angular_w[angular_off]);
    }

    // obtain radial parameters
    double r_inner = get_r_inner(radial_precision,
                                 alpha_max * 2.0); // factor 2.0 to match DIRAC
    double h = std::numeric_limits<float>::max();
    double r_outer = 0.0;
    for (int l = 0; l <= max_l_quantum_number; l++)
    {
        if (alpha_min[l] > 0.0)
        {
            r_outer =
                std::max(r_outer,
                         get_r_outer(radial_precision,
                                     alpha_min[l],
                                     l,
                                     4.0 * get_bragg_angstrom(proton_charge)));
            NUMGRID_ASSERT(r_outer > r_inner);
            h = std::min(h,
                         get_h(radial_precision, l, 0.1 * (r_outer - r_inner)));
        }
    }
    NUMGRID_ASSERT(r_outer > h);

    num_grid_points = 0;
    num_radial_grid_points = 0;

    double rb = get_bragg_angstrom(proton_charge) /
                (5.0 * 0.529177249); // factors match DIRAC code
    double c = r_inner / (exp(h) - 1.0);
    int num_radial = int(log(1.0 + (r_outer / c)) / h);
    for (int irad = 0; irad < num_radial; irad++)
    {
        double radial_r = c * (exp((irad + 1) * h) - 1.0);
        double radial_w = (radial_r + c) * radial_r * radial_r * h;

        radial_atom_grid_r_au.push_back(radial_r);
        radial_atom_grid_w.push_back(radial_w);
        num_radial_grid_points++;

        int num_angular = max_num_angular_points_closest;
        if (radial_r < rb)
        {
            num_angular = static_cast<int>(max_num_angular_points_closest *
                                           (radial_r / rb));
            num_angular = get_closest_num_angular(num_angular);
            if (num_angular < min_num_angular_points_closest)
                num_angular = min_num_angular_points_closest;
        }

        int angular_off = get_angular_order(num_angular) * MAX_ANGULAR_GRID;

        for (int iang = 0; iang < num_angular; iang++)
        {
            atom_grid_x_au.push_back(angular_x[angular_off + iang] * radial_r);
            atom_grid_y_au.push_back(angular_y[angular_off + iang] * radial_r);
            atom_grid_z_au.push_back(angular_z[angular_off + iang] * radial_r);

            atom_grid_w.push_back(4.0 * M_PI * angular_w[angular_off + iang] *
                                  radial_w);

            num_grid_points++;
        }
    }

    delete[] angular_x;
    delete[] angular_y;
    delete[] angular_z;
    delete[] angular_w;
}

void numgrid_free_atom_grid(context_t *context)
{
    if (!context)
        return;
    delete AS_TYPE(AtomGrid, context);
}
AtomGrid::~AtomGrid()
{
    atom_grid_x_au.clear();
    atom_grid_y_au.clear();
    atom_grid_z_au.clear();
    atom_grid_w.clear();

    radial_atom_grid_r_au.clear();
    radial_atom_grid_w.clear();
}

int numgrid_get_num_grid_points(const context_t *context)
{
    return AS_CTYPE(AtomGrid, context)->get_num_grid_points();
}
int AtomGrid::get_num_grid_points() const { return num_grid_points; }

int numgrid_get_num_radial_grid_points(const context_t *context)
{
    return AS_CTYPE(AtomGrid, context)->get_num_radial_grid_points();
}
int AtomGrid::get_num_radial_grid_points() const
{
    return num_radial_grid_points;
}

void numgrid_get_grid_points(const context_t *context,
                             const int num_centers,
                             const int center_index,
                             const double x_coordinates_au[],
                             const double y_coordinates_au[],
                             const double z_coordinates_au[],
                             const int proton_charges[],
                             double grid_x_au[],
                             double grid_y_au[],
                             double grid_z_au[],
                             double grid_w[])
{
    return AS_CTYPE(AtomGrid, context)
        ->get_grid_points(num_centers,
                          center_index,
                          x_coordinates_au,
                          y_coordinates_au,
                          z_coordinates_au,
                          proton_charges,
                          grid_x_au,
                          grid_y_au,
                          grid_z_au,
                          grid_w);
}
void AtomGrid::get_grid_points(const int num_centers,
                               const int center_index,
                               const double x_coordinates_au[],
                               const double y_coordinates_au[],
                               const double z_coordinates_au[],
                               const int proton_charges[],
                               double grid_x_au[],
                               double grid_y_au[],
                               double grid_z_au[],
                               double grid_w[]) const
{
    for (int ipoint = 0; ipoint < num_grid_points; ipoint++)
    {
        grid_x_au[ipoint] =
            atom_grid_x_au[ipoint] + x_coordinates_au[center_index];
        grid_y_au[ipoint] =
            atom_grid_y_au[ipoint] + y_coordinates_au[center_index];
        grid_z_au[ipoint] =
            atom_grid_z_au[ipoint] + z_coordinates_au[center_index];

        double becke_w = 1.0;

        // if there are no other centers
        // no point in partitioning the space
        if (num_centers > 1)
        {
            becke_w = get_becke_w(num_centers,
                                  proton_charges,
                                  x_coordinates_au,
                                  y_coordinates_au,
                                  z_coordinates_au,
                                  center_index,
                                  grid_x_au[ipoint],
                                  grid_y_au[ipoint],
                                  grid_z_au[ipoint]);
        }

        grid_w[ipoint] = atom_grid_w[ipoint] * becke_w;
    }
}

void numgrid_get_radial_grid_points(const context_t *context,
                                    double grid_r_au[],
                                    double grid_w[])
{
    return AS_CTYPE(AtomGrid, context)
        ->get_radial_grid_points(grid_r_au, grid_w);
}
void AtomGrid::get_radial_grid_points(double grid_r_au[], double grid_w[]) const
{
    for (int ipoint = 0; ipoint < num_radial_grid_points; ipoint++)
    {
        grid_r_au[ipoint] = radial_atom_grid_r_au[ipoint];
        grid_w[ipoint] = radial_atom_grid_w[ipoint];
    }
}
