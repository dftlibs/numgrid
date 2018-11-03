#include "gtest/gtest.h"
#include <cmath>
#include <fstream>

#include "numgrid.h"

TEST(numgrid, h2o_tabulated)
{
    double radial_precision = 1.0e-12;
    int min_num_angular_points = 86;
    int max_num_angular_points = 302;

    int num_centers = 3;

    int proton_charges[3];
    proton_charges[0] = 8;
    proton_charges[1] = 1;
    proton_charges[2] = 1;

    double x_coordinates_bohr[3];
    x_coordinates_bohr[0] = 0.0;
    x_coordinates_bohr[1] = 1.43;
    x_coordinates_bohr[2] = -1.43;

    double y_coordinates_bohr[3];
    y_coordinates_bohr[0] = 0.0;
    y_coordinates_bohr[1] = 0.0;
    y_coordinates_bohr[2] = 0.0;

    double z_coordinates_bohr[3];
    z_coordinates_bohr[0] = 0.0;
    z_coordinates_bohr[1] = 1.1;
    z_coordinates_bohr[2] = 1.1;

    int reference_num_points[3];
    reference_num_points[0] = 19310;
    reference_num_points[1] = 18132;
    reference_num_points[2] = 18132;

    std::ifstream infile("../test/reference_grid/dyall.v3z.txt");

    for (int center_index = 0; center_index < num_centers; center_index++)
    {
        context_t *context =
            numgrid_new_atom_grid_tabulated(radial_precision,
                                            min_num_angular_points,
                                            max_num_angular_points,
                                            proton_charges[center_index]);

        int num_points = numgrid_get_num_grid_points(context);
        ASSERT_EQ(num_points, reference_num_points[center_index]);

        double *grid_x_bohr = new double[num_points];
        double *grid_y_bohr = new double[num_points];
        double *grid_z_bohr = new double[num_points];
        double *grid_w = new double[num_points];

        numgrid_get_grid(context,
                         num_centers,
                         center_index,
                         x_coordinates_bohr,
                         y_coordinates_bohr,
                         z_coordinates_bohr,
                         proton_charges,
                         grid_x_bohr,
                         grid_y_bohr,
                         grid_z_bohr,
                         grid_w);

        double x_ref, y_ref, z_ref, w_ref;
        for (int ipoint = 0; ipoint < num_points; ipoint++)
        {
            infile >> x_ref >> y_ref >> z_ref >> w_ref;

            ASSERT_NEAR(x_ref, grid_x_bohr[ipoint], 1.0e-10);
            ASSERT_NEAR(y_ref, grid_y_bohr[ipoint], 1.0e-10);
            ASSERT_NEAR(z_ref, grid_z_bohr[ipoint], 1.0e-10);
            ASSERT_NEAR(w_ref, grid_w[ipoint], 1.0e-10);
        }

        delete[] grid_x_bohr;
        delete[] grid_y_bohr;
        delete[] grid_z_bohr;
        delete[] grid_w;

        numgrid_free_atom_grid(context);
    }
}
