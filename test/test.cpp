#include <fstream>
#include <cmath>
#include "gtest/gtest.h"

#include "numgrid.h"


// H2O grid
TEST(numgrid, h2o)
{
    double radial_precision = 1.000000e-12;
    int angular_min = 86;
    int angular_max = 302;
    int num_centers = 3;
    double center_coordinates[num_centers*3];
    center_coordinates[0] = 0.000000e+00;
    center_coordinates[1] = 0.000000e+00;
    center_coordinates[2] = 0.000000e+00;
    center_coordinates[3] = 5.516800e-01;
    center_coordinates[4] = 7.734000e-01;
    center_coordinates[5] = 0.000000e+00;
    center_coordinates[6] = 5.516800e-01;
    center_coordinates[7] = -7.734000e-01;
    center_coordinates[8] = 0.000000e+00;
    int center_elements[num_centers];
    center_elements[0] = 8;
    center_elements[1] = 1;
    center_elements[2] = 1;
    int num_outer_centers = 0;
    double *outer_center_coordinates = NULL;
    int *outer_center_elements = NULL;
    int num_shells = 12;
    int shell_centers[num_shells];
    shell_centers[ 0] = 1;
    shell_centers[ 1] = 1;
    shell_centers[ 2] = 1;
    shell_centers[ 3] = 1;
    shell_centers[ 4] = 1;
    shell_centers[ 5] = 1;
    shell_centers[ 6] = 2;
    shell_centers[ 7] = 2;
    shell_centers[ 8] = 2;
    shell_centers[ 9] = 3;
    shell_centers[10] = 3;
    shell_centers[11] = 3;
    int l_quantum_numbers[num_shells];
    l_quantum_numbers[ 0] = 0;
    l_quantum_numbers[ 1] = 0;
    l_quantum_numbers[ 2] = 0;
    l_quantum_numbers[ 3] = 1;
    l_quantum_numbers[ 4] = 1;
    l_quantum_numbers[ 5] = 2;
    l_quantum_numbers[ 6] = 0;
    l_quantum_numbers[ 7] = 0;
    l_quantum_numbers[ 8] = 1;
    l_quantum_numbers[ 9] = 0;
    l_quantum_numbers[10] = 0;
    l_quantum_numbers[11] = 1;
    int shell_num_primitives[num_shells];
    shell_num_primitives[ 0] = 9;
    shell_num_primitives[ 1] = 9;
    shell_num_primitives[ 2] = 1;
    shell_num_primitives[ 3] = 4;
    shell_num_primitives[ 4] = 1;
    shell_num_primitives[ 5] = 1;
    shell_num_primitives[ 6] = 4;
    shell_num_primitives[ 7] = 1;
    shell_num_primitives[ 8] = 1;
    shell_num_primitives[ 9] = 4;
    shell_num_primitives[10] = 1;
    shell_num_primitives[11] = 1;
    double primitive_exponents[37];
    primitive_exponents[ 0] = 1.172000e+04;
    primitive_exponents[ 1] = 1.759000e+03;
    primitive_exponents[ 2] = 4.008000e+02;
    primitive_exponents[ 3] = 1.137000e+02;
    primitive_exponents[ 4] = 3.703000e+01;
    primitive_exponents[ 5] = 1.327000e+01;
    primitive_exponents[ 6] = 5.025000e+00;
    primitive_exponents[ 7] = 1.013000e+00;
    primitive_exponents[ 8] = 3.023000e-01;
    primitive_exponents[ 9] = 1.172000e+04;
    primitive_exponents[10] = 1.759000e+03;
    primitive_exponents[11] = 4.008000e+02;
    primitive_exponents[12] = 1.137000e+02;
    primitive_exponents[13] = 3.703000e+01;
    primitive_exponents[14] = 1.327000e+01;
    primitive_exponents[15] = 5.025000e+00;
    primitive_exponents[16] = 1.013000e+00;
    primitive_exponents[17] = 3.023000e-01;
    primitive_exponents[18] = 3.023000e-01;
    primitive_exponents[19] = 1.770000e+01;
    primitive_exponents[20] = 3.854000e+00;
    primitive_exponents[21] = 1.046000e+00;
    primitive_exponents[22] = 2.753000e-01;
    primitive_exponents[23] = 2.753000e-01;
    primitive_exponents[24] = 1.185000e+00;
    primitive_exponents[25] = 1.301000e+01;
    primitive_exponents[26] = 1.962000e+00;
    primitive_exponents[27] = 4.446000e-01;
    primitive_exponents[28] = 1.220000e-01;
    primitive_exponents[29] = 1.220000e-01;
    primitive_exponents[30] = 7.270000e-01;
    primitive_exponents[31] = 1.301000e+01;
    primitive_exponents[32] = 1.962000e+00;
    primitive_exponents[33] = 4.446000e-01;
    primitive_exponents[34] = 1.220000e-01;
    primitive_exponents[35] = 1.220000e-01;
    primitive_exponents[36] = 7.270000e-01;

    numgrid_context_t *context = numgrid_new();

    int ierr = numgrid_generate(context,
                                radial_precision,
                                angular_min,
                                angular_max,
                                num_centers,
                                center_coordinates,
                                center_elements,
                                num_outer_centers,
                                outer_center_coordinates,
                                outer_center_elements,
                                num_shells,
                                shell_centers,
                                l_quantum_numbers,
                                shell_num_primitives,
                                primitive_exponents);

    int num_points = numgrid_get_num_points(context);
    ASSERT_EQ(num_points, 46220);

    double *grid_pw = (double*) numgrid_get_grid(context);
    std::ifstream infile("../test/referece_grid.txt");
    int i = 0;
    double ref[4];
    while (infile >> ref[0] >> ref[1] >> ref[2] >> ref[3])
    {
        for (int j = 0; j < 4; j++)
        {
            double error = grid_pw[i++] - ref[j];
            if (fabs(ref[j]) > 1.0e-20) error /= ref[j];
            ASSERT_TRUE(fabs(error) < 1.0e-6);
        }
    }

    numgrid_free(context);
}


// O grid in the presence of two H (in H2O)
TEST(numgrid, o_in_h2o)
{
    double radial_precision = 1.000000e-12;
    int angular_min = 86;
    int angular_max = 302;
    int num_centers = 1;
    double center_coordinates[num_centers*3];
    center_coordinates[0] = 0.000000e+00;
    center_coordinates[1] = 0.000000e+00;
    center_coordinates[2] = 0.000000e+00;
    int center_elements[num_centers];
    center_elements[0] = 8;
    int num_outer_centers = 2;
    double outer_center_coordinates[num_outer_centers*3];
    outer_center_coordinates[0] = 5.516800e-01;
    outer_center_coordinates[1] = 7.734000e-01;
    outer_center_coordinates[2] = 0.000000e+00;
    outer_center_coordinates[3] = 5.516800e-01;
    outer_center_coordinates[4] = -7.734000e-01;
    outer_center_coordinates[5] = 0.000000e+00;
    int outer_center_elements[num_outer_centers];
    outer_center_elements[0] = 1;
    outer_center_elements[1] = 1;
    int num_shells = 6;
    int shell_centers[num_shells];
    shell_centers[0] = 1;
    shell_centers[1] = 1;
    shell_centers[2] = 1;
    shell_centers[3] = 1;
    shell_centers[4] = 1;
    shell_centers[5] = 1;
    int l_quantum_numbers[num_shells];
    l_quantum_numbers[0] = 0;
    l_quantum_numbers[1] = 0;
    l_quantum_numbers[2] = 0;
    l_quantum_numbers[3] = 1;
    l_quantum_numbers[4] = 1;
    l_quantum_numbers[5] = 2;
    int shell_num_primitives[num_shells];
    shell_num_primitives[0] = 9;
    shell_num_primitives[1] = 9;
    shell_num_primitives[2] = 1;
    shell_num_primitives[3] = 4;
    shell_num_primitives[4] = 1;
    shell_num_primitives[5] = 1;
    double primitive_exponents[25];
    primitive_exponents[ 0] = 1.172000e+04;
    primitive_exponents[ 1] = 1.759000e+03;
    primitive_exponents[ 2] = 4.008000e+02;
    primitive_exponents[ 3] = 1.137000e+02;
    primitive_exponents[ 4] = 3.703000e+01;
    primitive_exponents[ 5] = 1.327000e+01;
    primitive_exponents[ 6] = 5.025000e+00;
    primitive_exponents[ 7] = 1.013000e+00;
    primitive_exponents[ 8] = 3.023000e-01;
    primitive_exponents[ 9] = 1.172000e+04;
    primitive_exponents[10] = 1.759000e+03;
    primitive_exponents[11] = 4.008000e+02;
    primitive_exponents[12] = 1.137000e+02;
    primitive_exponents[13] = 3.703000e+01;
    primitive_exponents[14] = 1.327000e+01;
    primitive_exponents[15] = 5.025000e+00;
    primitive_exponents[16] = 1.013000e+00;
    primitive_exponents[17] = 3.023000e-01;
    primitive_exponents[18] = 3.023000e-01;
    primitive_exponents[19] = 1.770000e+01;
    primitive_exponents[20] = 3.854000e+00;
    primitive_exponents[21] = 1.046000e+00;
    primitive_exponents[22] = 2.753000e-01;
    primitive_exponents[23] = 2.753000e-01;
    primitive_exponents[24] = 1.185000e+00;

    numgrid_context_t *context = numgrid_new();

    int ierr = numgrid_generate(context,
                                radial_precision,
                                angular_min,
                                angular_max,
                                num_centers,
                                center_coordinates,
                                center_elements,
                                num_outer_centers,
                                outer_center_coordinates,
                                outer_center_elements,
                                num_shells,
                                shell_centers,
                                l_quantum_numbers,
                                shell_num_primitives,
                                primitive_exponents);

    int num_points = numgrid_get_num_points(context);
    ASSERT_EQ(num_points, 16364);

    double *grid_pw = (double*) numgrid_get_grid(context);
    std::ifstream infile("../test/referece_grid.txt");
    int i = 0;
    double ref[4];
    while (infile >> ref[0] >> ref[1] >> ref[2] >> ref[3])
    {
        for (int j = 0; j < 4; j++)
        {
            double error = grid_pw[i++] - ref[j];
            if (fabs(ref[j]) > 1.0e-20) error /= ref[j];
            ASSERT_TRUE(fabs(error) < 1.0e-6);
        }
        if (i == num_points*4) break;
    }

    numgrid_free(context);
}
