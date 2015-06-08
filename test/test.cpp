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
    double center_xyz[num_centers*3];
    center_xyz[0] = 0.000000e+00;
    center_xyz[1] = 0.000000e+00;
    center_xyz[2] = 0.000000e+00;
    center_xyz[3] = 5.516800e-01;
    center_xyz[4] = 7.734000e-01;
    center_xyz[5] = 0.000000e+00;
    center_xyz[6] = 5.516800e-01;
    center_xyz[7] = -7.734000e-01;
    center_xyz[8] = 0.000000e+00;
    int center_element[num_centers];
    center_element[0] = 8;
    center_element[1] = 1;
    center_element[2] = 1;
    int num_outer_centers = 0;
    double *outer_center_xyz = NULL;
    int *outer_center_element = NULL;
    int num_shells = 12;
    int shell_center[num_shells];
    shell_center[ 0] = 1;
    shell_center[ 1] = 1;
    shell_center[ 2] = 1;
    shell_center[ 3] = 1;
    shell_center[ 4] = 1;
    shell_center[ 5] = 1;
    shell_center[ 6] = 2;
    shell_center[ 7] = 2;
    shell_center[ 8] = 2;
    shell_center[ 9] = 3;
    shell_center[10] = 3;
    shell_center[11] = 3;
    int l_quantum_num[num_shells];
    l_quantum_num[ 0] = 0;
    l_quantum_num[ 1] = 0;
    l_quantum_num[ 2] = 0;
    l_quantum_num[ 3] = 1;
    l_quantum_num[ 4] = 1;
    l_quantum_num[ 5] = 2;
    l_quantum_num[ 6] = 0;
    l_quantum_num[ 7] = 0;
    l_quantum_num[ 8] = 1;
    l_quantum_num[ 9] = 0;
    l_quantum_num[10] = 0;
    l_quantum_num[11] = 1;
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
    double primitive_exp[37];
    primitive_exp[ 0] = 1.172000e+04;
    primitive_exp[ 1] = 1.759000e+03;
    primitive_exp[ 2] = 4.008000e+02;
    primitive_exp[ 3] = 1.137000e+02;
    primitive_exp[ 4] = 3.703000e+01;
    primitive_exp[ 5] = 1.327000e+01;
    primitive_exp[ 6] = 5.025000e+00;
    primitive_exp[ 7] = 1.013000e+00;
    primitive_exp[ 8] = 3.023000e-01;
    primitive_exp[ 9] = 1.172000e+04;
    primitive_exp[10] = 1.759000e+03;
    primitive_exp[11] = 4.008000e+02;
    primitive_exp[12] = 1.137000e+02;
    primitive_exp[13] = 3.703000e+01;
    primitive_exp[14] = 1.327000e+01;
    primitive_exp[15] = 5.025000e+00;
    primitive_exp[16] = 1.013000e+00;
    primitive_exp[17] = 3.023000e-01;
    primitive_exp[18] = 3.023000e-01;
    primitive_exp[19] = 1.770000e+01;
    primitive_exp[20] = 3.854000e+00;
    primitive_exp[21] = 1.046000e+00;
    primitive_exp[22] = 2.753000e-01;
    primitive_exp[23] = 2.753000e-01;
    primitive_exp[24] = 1.185000e+00;
    primitive_exp[25] = 1.301000e+01;
    primitive_exp[26] = 1.962000e+00;
    primitive_exp[27] = 4.446000e-01;
    primitive_exp[28] = 1.220000e-01;
    primitive_exp[29] = 1.220000e-01;
    primitive_exp[30] = 7.270000e-01;
    primitive_exp[31] = 1.301000e+01;
    primitive_exp[32] = 1.962000e+00;
    primitive_exp[33] = 4.446000e-01;
    primitive_exp[34] = 1.220000e-01;
    primitive_exp[35] = 1.220000e-01;
    primitive_exp[36] = 7.270000e-01;

    numgrid_context_t *context = numgrid_new();

    numgrid_generate(context,
                     radial_precision,
                     angular_min,
                     angular_max,
                     num_centers,
                     center_xyz,
                     center_element,
                     num_outer_centers,
                     outer_center_xyz,
                     outer_center_element,
                     num_shells,
                     shell_center,
                     l_quantum_num,
                     shell_num_primitives,
                     primitive_exp);

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
    double center_xyz[num_centers*3];
    center_xyz[0] = 0.000000e+00;
    center_xyz[1] = 0.000000e+00;
    center_xyz[2] = 0.000000e+00;
    int center_element[num_centers];
    center_element[0] = 8;
    int num_outer_centers = 2;
    double outer_center_xyz[num_outer_centers*3];
    outer_center_xyz[0] = 5.516800e-01;
    outer_center_xyz[1] = 7.734000e-01;
    outer_center_xyz[2] = 0.000000e+00;
    outer_center_xyz[3] = 5.516800e-01;
    outer_center_xyz[4] = -7.734000e-01;
    outer_center_xyz[5] = 0.000000e+00;
    int outer_center_element[num_outer_centers];
    outer_center_element[0] = 1;
    outer_center_element[1] = 1;
    int num_shells = 6;
    int shell_center[num_shells];
    shell_center[ 0] = 1;
    shell_center[ 1] = 1;
    shell_center[ 2] = 1;
    shell_center[ 3] = 1;
    shell_center[ 4] = 1;
    shell_center[ 5] = 1;
    int l_quantum_num[num_shells];
    l_quantum_num[ 0] = 0;
    l_quantum_num[ 1] = 0;
    l_quantum_num[ 2] = 0;
    l_quantum_num[ 3] = 1;
    l_quantum_num[ 4] = 1;
    l_quantum_num[ 5] = 2;
    int shell_num_primitives[num_shells];
    shell_num_primitives[ 0] = 9;
    shell_num_primitives[ 1] = 9;
    shell_num_primitives[ 2] = 1;
    shell_num_primitives[ 3] = 4;
    shell_num_primitives[ 4] = 1;
    shell_num_primitives[ 5] = 1;
    double primitive_exp[25];
    primitive_exp[ 0] = 1.172000e+04;
    primitive_exp[ 1] = 1.759000e+03;
    primitive_exp[ 2] = 4.008000e+02;
    primitive_exp[ 3] = 1.137000e+02;
    primitive_exp[ 4] = 3.703000e+01;
    primitive_exp[ 5] = 1.327000e+01;
    primitive_exp[ 6] = 5.025000e+00;
    primitive_exp[ 7] = 1.013000e+00;
    primitive_exp[ 8] = 3.023000e-01;
    primitive_exp[ 9] = 1.172000e+04;
    primitive_exp[10] = 1.759000e+03;
    primitive_exp[11] = 4.008000e+02;
    primitive_exp[12] = 1.137000e+02;
    primitive_exp[13] = 3.703000e+01;
    primitive_exp[14] = 1.327000e+01;
    primitive_exp[15] = 5.025000e+00;
    primitive_exp[16] = 1.013000e+00;
    primitive_exp[17] = 3.023000e-01;
    primitive_exp[18] = 3.023000e-01;
    primitive_exp[19] = 1.770000e+01;
    primitive_exp[20] = 3.854000e+00;
    primitive_exp[21] = 1.046000e+00;
    primitive_exp[22] = 2.753000e-01;
    primitive_exp[23] = 2.753000e-01;
    primitive_exp[24] = 1.185000e+00;

    numgrid_context_t *context = numgrid_new();

    numgrid_generate(context,
                     radial_precision,
                     angular_min,
                     angular_max,
                     num_centers,
                     center_xyz,
                     center_element,
                     num_outer_centers,
                     outer_center_xyz,
                     outer_center_element,
                     num_shells,
                     shell_center,
                     l_quantum_num,
                     shell_num_primitives,
                     primitive_exp);

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
