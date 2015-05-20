#ifndef numgrid_c_api_h_
#define numgrid_c_api_h_

extern "C"
{
void numgrid_set_grid_parameters(const double radial_precision,
                                 const int    angular_min,
                                 const int    angular_max);

void numgrid_generate(const int    verbosity,
                      const int    num_centers,
                      const double center_xyz[],
                      const int    center_element[],
                      const int    num_shells,
                      const int    shell_center[],
                      const int    l_quantum_num[],
                      const int    shell_num_primitives[],
                      const double primitive_exp[]);

int numgrid_get_num_points();

double *numgrid_get_grid_w();

double *numgrid_get_grid_p();
}

#endif // numgrid_c_api_h_
