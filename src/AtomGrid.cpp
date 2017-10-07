#include "AtomGrid.h"
#include "becke_partitioning.h"
#include "error_handling.h"
#include "grid_radial.h"
#include "lebedev/sphere_lebedev_rule.h"
#include "parameters.h"
#include <cmath>

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

AtomGrid::AtomGrid(const double radial_precision,
                   const int min_num_angular_points,
                   const int max_num_angular_points,
                   const int proton_charge,
                   const int num_shells,
                   const int shell_l_quantum_numbers[],
                   const int shell_num_primitives[],
                   const double primitive_exponents[])
{
    int num_min_num_angular_points = // FIXME rename
        get_closest_num_angular(min_num_angular_points);
    int num_max_num_angular_points =
        get_closest_num_angular(max_num_angular_points);

    double *angular_x = new double[MAX_ANGULAR_ORDER * MAX_ANGULAR_GRID];
    double *angular_y = new double[MAX_ANGULAR_ORDER * MAX_ANGULAR_GRID];
    double *angular_z = new double[MAX_ANGULAR_ORDER * MAX_ANGULAR_GRID];
    double *angular_w = new double[MAX_ANGULAR_ORDER * MAX_ANGULAR_GRID];

    for (int i = get_angular_order(num_min_num_angular_points);
         i <= get_angular_order(num_max_num_angular_points);
         i++)
    {
        int angular_off = i * MAX_ANGULAR_GRID;
        ld_by_order(lebedev_table[i],
                    &angular_x[angular_off],
                    &angular_y[angular_off],
                    &angular_z[angular_off],
                    &angular_w[angular_off]);
    }

    int l_max = 0;
    for (int ishell = 0; ishell < num_shells; ishell++)
    {
        l_max = std::max(l_max, shell_l_quantum_numbers[ishell]);
    }

    // get extreme alpha values
    double alpha_max = 0.0;
    double *alpha_min = new double[l_max + 1];
    bool *alpha_min_set = new bool[l_max + 1];
    std::fill(&alpha_min[0], &alpha_min[l_max + 1], 0.0);
    std::fill(&alpha_min_set[0], &alpha_min_set[l_max + 1], false);

    int n = 0;
    for (int ishell = 0; ishell < num_shells; ishell++)
    {
        int l = shell_l_quantum_numbers[ishell];

        if (!alpha_min_set[l])
        {
            alpha_min[l] = 1.0e50;
            alpha_min_set[l] = true;
        }

        for (int p = 0; p < shell_num_primitives[ishell]; p++)
        {
            double e = primitive_exponents[n + p];
            alpha_max =
                std::max(alpha_max, 2.0 * e); // factor 2.0 to match DIRAC
            alpha_min[l] = std::min(alpha_min[l], e);
        }
        n += shell_num_primitives[ishell];
    }

    // obtain radial parameters
    double r_inner = get_r_inner(radial_precision, alpha_max);
    double h = 1.0e50;
    double r_outer = 0.0;
    for (int l = 0; l <= l_max; l++)
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

    delete[] alpha_min;
    delete[] alpha_min_set;

    num_atom_grid_points = 0;

    double rb = get_bragg_angstrom(proton_charge) /
                (5.0 * 0.529177249); // factors match DIRAC code
    double c = r_inner / (exp(h) - 1.0);
    int num_radial = int(log(1.0 + (r_outer / c)) / h);
    for (int irad = 0; irad < num_radial; irad++)
    {
        double radial_r = c * (exp((irad + 1) * h) - 1.0);
        double radial_w = (radial_r + c) * radial_r * radial_r * h;

        int num_angular = num_max_num_angular_points;
        if (radial_r < rb)
        {
            num_angular =
                static_cast<int>(num_max_num_angular_points * (radial_r / rb));
            num_angular = get_closest_num_angular(num_angular);
            if (num_angular < num_min_num_angular_points)
                num_angular = num_min_num_angular_points;
        }

        int angular_off = get_angular_order(num_angular) * MAX_ANGULAR_GRID;

        for (int iang = 0; iang < num_angular; iang++)
        {
            atom_grid_x_au.push_back(angular_x[angular_off + iang] * radial_r);
            atom_grid_y_au.push_back(angular_y[angular_off + iang] * radial_r);
            atom_grid_z_au.push_back(angular_z[angular_off + iang] * radial_r);

            atom_grid_w.push_back(4.0 * M_PI * angular_w[angular_off + iang] *
                                  radial_w);

            num_atom_grid_points++;
        }
    }

    delete[] angular_x;
    delete[] angular_y;
    delete[] angular_z;
    delete[] angular_w;
}

AtomGrid::~AtomGrid()
{
    atom_grid_x_au.clear();
    atom_grid_y_au.clear();
    atom_grid_z_au.clear();
    atom_grid_w.clear();
}

int AtomGrid::get_num_grid_points() const { return num_atom_grid_points; }

void AtomGrid::get_grid_points(double grid_x_au[],
                               double grid_y_au[],
                               double grid_z_au[],
                               double grid_w[],
                               const int center_index,
                               const int num_centers,
                               const double x_coordinates_au[],
                               const double y_coordinates_au[],
                               const double z_coordinates_au[],
                               const int proton_charges[]) const
{
    double becke_w = 1.0;
    double *pa_buffer = new double[num_centers];

    for (int ipoint = 0; ipoint < num_atom_grid_points; ipoint++)
    {
        grid_x_au[ipoint] =
            atom_grid_x_au[ipoint] + x_coordinates_au[center_index];
        grid_y_au[ipoint] =
            atom_grid_y_au[ipoint] + y_coordinates_au[center_index];
        grid_z_au[ipoint] =
            atom_grid_z_au[ipoint] + z_coordinates_au[center_index];

        if (num_centers > 1)
        {
            becke_w = get_becke_w(x_coordinates_au,
                                  y_coordinates_au,
                                  z_coordinates_au,
                                  proton_charges,
                                  pa_buffer,
                                  center_index,
                                  num_centers,
                                  grid_x_au[ipoint],
                                  grid_y_au[ipoint],
                                  grid_z_au[ipoint]);
        }

        grid_w[ipoint] = atom_grid_w[ipoint] * becke_w;
    }
    delete[] pa_buffer;
}
