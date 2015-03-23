
#include "numgrid_c_interface.h"
#include "Grid.h"


Grid grid;


void numgrid_set_grid_parameters(const double radial_precision,
                                  const int    angular_min,
                                  const int    angular_max)
{
    grid.set_grid_parameters(radial_precision,
                             angular_min,
                             angular_max);
}


void numgrid_generate(const int    verbosity,
                       const int    num_centers,
                       const double center_xyz[],
                       const int    center_element[],
                       const int    num_shells,
                       const int    shell_center[],
                       const int    l_quantum_num[],
                       const int    shell_num_primitives[],
                       const double primitive_exp[])
{
    grid.generate(verbosity,
                  num_centers,
                  center_xyz,
                  center_element,
                  num_shells,
                  shell_center,
                  l_quantum_num,
                  shell_num_primitives,
                  primitive_exp);
}


void numgrid_read()
{
    grid.read();
}


void numgrid_stretch()
{
    grid.stretch();
}


int numgrid_get_num_points()
{
    return grid.num_points;
}


double *numgrid_get_grid_w()
{
    return grid.w;
}


double *numgrid_get_grid_p()
{
    return grid.p;
}


#ifdef ENABLE_MPI
void numgrid_distribute(const MPI_Comm &comm)
{
    grid.distribute(comm);
}
#endif
