#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

class Grid
{
    public:

        Grid();
        ~Grid();

        double *pw;
        int    num_points;

        void generate(const double radial_precision,
                      const int    angular_min,
                      const int    angular_max,
                      const int    num_centers,
                      const double center_xyz[],
                      const int    center_element[],
                      const int    num_shells,
                      const int    shell_center[],
                      const int    l_quantum_num[],
                      const int    shell_num_primitives[],
                      const double primitive_exp[]);

    private:

        Grid(const Grid &rhs);            // not implemented
        Grid &operator=(const Grid &rhs); // not implemented

        void nullify();

        int get_closest_num_angular(int n);
        int get_angular_order(int n);
};

#endif
