#ifndef Grid_h_
#define Grid_h_

#ifdef ENABLE_MPI
#include "mpi.h"
#endif

class Grid
{
    public:

        Grid();
        ~Grid();

        double *p;
        double *w;
        int    num_points;

        void read();

        void stretch();

        void generate(const int    verbosity,
                      const int    num_centers,
                      const double center_xyz[],
                      const int    center_element[],
                      const int    num_shells,
                      const int    shell_center[],
                      const int    l_quantum_num[],
                      const int    shell_num_primitives[],
                      const double primitive_exp[]);

        void set_grid_parameters(const double in_radial_precision,
                                 const int    in_angular_min,
                                 const int    in_angular_max);

#ifdef ENABLE_MPI
        void distribute(const MPI_Comm &comm);
#endif

    private:

        Grid(const Grid &rhs);            // not implemented
        Grid &operator=(const Grid &rhs); // not implemented

        void generate_sub(const int    num_centers,
                          const double center_xyz[],
                          const int    center_element[],
                          const int    num_shells,
                          const int    shell_center[],
                          const int    l_quantum_num[],
                          const int    shell_num_primitives[],
                          const double primitive_exp[],
                          const int    num_angular_min,
                          const int    num_angular_max,
                          const double angular_x[],
                          const double angular_y[],
                          const double angular_z[],
                          const double angular_w[]);

        void nullify();

        bool is_generated;
        bool is_distributed;

        int get_closest_num_angular(int n);
        int get_angular_order(int n);

        double radial_precision;
        int    angular_min;
        int    angular_max;
};

#endif // Grid_h_
