#include <fstream>
#include <cmath>
#include "gtest/gtest.h"

#include "numgrid.h"


// H2O grid
TEST(numgrid, h2o)
{
    double radial_precision = 1.0e-12;
    int min_num_angular_points = 86;
    int max_num_angular_points = 302;
    int num_centers = 3;
    double center_coordinates[num_centers*3];
    center_coordinates[0] =  0.00;
    center_coordinates[1] =  0.00;
    center_coordinates[2] =  0.00;
    center_coordinates[3] =  1.43;
    center_coordinates[4] =  0.00;
    center_coordinates[5] =  1.10;
    center_coordinates[6] = -1.43;
    center_coordinates[7] =  0.00;
    center_coordinates[8] =  1.10;
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
    int shell_l_quantum_numbers[num_shells];
    shell_l_quantum_numbers[ 0] = 0;
    shell_l_quantum_numbers[ 1] = 0;
    shell_l_quantum_numbers[ 2] = 0;
    shell_l_quantum_numbers[ 3] = 1;
    shell_l_quantum_numbers[ 4] = 1;
    shell_l_quantum_numbers[ 5] = 2;
    shell_l_quantum_numbers[ 6] = 0;
    shell_l_quantum_numbers[ 7] = 0;
    shell_l_quantum_numbers[ 8] = 1;
    shell_l_quantum_numbers[ 9] = 0;
    shell_l_quantum_numbers[10] = 0;
    shell_l_quantum_numbers[11] = 1;
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
    primitive_exponents[ 0] = 1.172e+04;
    primitive_exponents[ 1] = 1.759e+03;
    primitive_exponents[ 2] = 4.008e+02;
    primitive_exponents[ 3] = 1.137e+02;
    primitive_exponents[ 4] = 3.703e+01;
    primitive_exponents[ 5] = 1.327e+01;
    primitive_exponents[ 6] = 5.025e+00;
    primitive_exponents[ 7] = 1.013e+00;
    primitive_exponents[ 8] = 3.023e-01;
    primitive_exponents[ 9] = 1.172e+04;
    primitive_exponents[10] = 1.759e+03;
    primitive_exponents[11] = 4.008e+02;
    primitive_exponents[12] = 1.137e+02;
    primitive_exponents[13] = 3.703e+01;
    primitive_exponents[14] = 1.327e+01;
    primitive_exponents[15] = 5.025e+00;
    primitive_exponents[16] = 1.013e+00;
    primitive_exponents[17] = 3.023e-01;
    primitive_exponents[18] = 3.023e-01;
    primitive_exponents[19] = 1.770e+01;
    primitive_exponents[20] = 3.854e+00;
    primitive_exponents[21] = 1.046e+00;
    primitive_exponents[22] = 2.753e-01;
    primitive_exponents[23] = 2.753e-01;
    primitive_exponents[24] = 1.185e+00;
    primitive_exponents[25] = 1.301e+01;
    primitive_exponents[26] = 1.962e+00;
    primitive_exponents[27] = 4.446e-01;
    primitive_exponents[28] = 1.220e-01;
    primitive_exponents[29] = 1.220e-01;
    primitive_exponents[30] = 7.270e-01;
    primitive_exponents[31] = 1.301e+01;
    primitive_exponents[32] = 1.962e+00;
    primitive_exponents[33] = 4.446e-01;
    primitive_exponents[34] = 1.220e-01;
    primitive_exponents[35] = 1.220e-01;
    primitive_exponents[36] = 7.270e-01;

    context_t *context = numgrid_new_context();

    int ierr = numgrid_generate_grid(context,
                             radial_precision,
                             min_num_angular_points,
                             max_num_angular_points,
                             num_centers,
                             center_coordinates,
                             center_elements,
                             num_outer_centers,
                             outer_center_coordinates,
                             outer_center_elements,
                             num_shells,
                             shell_centers,
                             shell_l_quantum_numbers,
                             shell_num_primitives,
                             primitive_exponents);

    int num_points = numgrid_get_num_points(context);
    ASSERT_EQ(num_points, 46220);

    double *grid_pw = (double*) numgrid_get_grid(context);
    std::ifstream infile("../test/reference_grid.txt");
    int i = 0;
    double ref[4];
    while (infile >> ref[0] >> ref[1] >> ref[2] >> ref[3])
    {
        for (int j = 0; j < 4; j++)
        {
            double error = grid_pw[i++] - ref[j];
            if (fabs(ref[j]) > 1.0e-15) error /= ref[j];
            ASSERT_TRUE(fabs(error) < 1.0e-5);
        }
    }

    numgrid_free_context(context);
}


// O grid in the presence of two H (in H2O)
TEST(numgrid, o_in_h2o)
{
    double radial_precision = 1.0e-12;
    int min_num_angular_points = 86;
    int max_num_angular_points = 302;
    int num_centers = 1;
    double center_coordinates[num_centers*3];
    center_coordinates[0] = 0.0;
    center_coordinates[1] = 0.0;
    center_coordinates[2] = 0.0;
    int center_elements[num_centers];
    center_elements[0] = 8;
    int num_outer_centers = 2;
    double outer_center_coordinates[num_outer_centers*3];
    outer_center_coordinates[0] =  1.43;
    outer_center_coordinates[1] =  0.00;
    outer_center_coordinates[2] =  1.10;
    outer_center_coordinates[3] = -1.43;
    outer_center_coordinates[4] =  0.00;
    outer_center_coordinates[5] =  1.10;
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
    int shell_l_quantum_numbers[num_shells];
    shell_l_quantum_numbers[0] = 0;
    shell_l_quantum_numbers[1] = 0;
    shell_l_quantum_numbers[2] = 0;
    shell_l_quantum_numbers[3] = 1;
    shell_l_quantum_numbers[4] = 1;
    shell_l_quantum_numbers[5] = 2;
    int shell_num_primitives[num_shells];
    shell_num_primitives[0] = 9;
    shell_num_primitives[1] = 9;
    shell_num_primitives[2] = 1;
    shell_num_primitives[3] = 4;
    shell_num_primitives[4] = 1;
    shell_num_primitives[5] = 1;
    double primitive_exponents[25];
    primitive_exponents[ 0] = 1.172e+04;
    primitive_exponents[ 1] = 1.759e+03;
    primitive_exponents[ 2] = 4.008e+02;
    primitive_exponents[ 3] = 1.137e+02;
    primitive_exponents[ 4] = 3.703e+01;
    primitive_exponents[ 5] = 1.327e+01;
    primitive_exponents[ 6] = 5.025e+00;
    primitive_exponents[ 7] = 1.013e+00;
    primitive_exponents[ 8] = 3.023e-01;
    primitive_exponents[ 9] = 1.172e+04;
    primitive_exponents[10] = 1.759e+03;
    primitive_exponents[11] = 4.008e+02;
    primitive_exponents[12] = 1.137e+02;
    primitive_exponents[13] = 3.703e+01;
    primitive_exponents[14] = 1.327e+01;
    primitive_exponents[15] = 5.025e+00;
    primitive_exponents[16] = 1.013e+00;
    primitive_exponents[17] = 3.023e-01;
    primitive_exponents[18] = 3.023e-01;
    primitive_exponents[19] = 1.770e+01;
    primitive_exponents[20] = 3.854e+00;
    primitive_exponents[21] = 1.046e+00;
    primitive_exponents[22] = 2.753e-01;
    primitive_exponents[23] = 2.753e-01;
    primitive_exponents[24] = 1.185e+00;

    context_t *context = numgrid_new_context();

    int ierr = numgrid_generate_grid(context,
                             radial_precision,
                             min_num_angular_points,
                             max_num_angular_points,
                             num_centers,
                             center_coordinates,
                             center_elements,
                             num_outer_centers,
                             outer_center_coordinates,
                             outer_center_elements,
                             num_shells,
                             shell_centers,
                             shell_l_quantum_numbers,
                             shell_num_primitives,
                             primitive_exponents);

    int num_points = numgrid_get_num_points(context);
    ASSERT_EQ(num_points, 16364);

    double *grid_pw = (double*) numgrid_get_grid(context);
    std::ifstream infile("../test/reference_grid.txt");
    int i = 0;
    double ref[4];
    while (infile >> ref[0] >> ref[1] >> ref[2] >> ref[3])
    {
        for (int j = 0; j < 4; j++)
        {
            double error = grid_pw[i++] - ref[j];
            if (fabs(ref[j]) > 1.0e-15) error /= ref[j];
            ASSERT_TRUE(fabs(error) < 1.0e-5);
        }
        if (i == num_points*4) break;
    }

    numgrid_free_context(context);
}
