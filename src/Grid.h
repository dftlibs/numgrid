#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

class Grid
{
    public:

        Grid();
        ~Grid();

        void generate(const double radial_precision,
                      const int    angular_min,
                      const int    angular_max,
                      const int    num_centers,
                      const double center_xyz[],
                      const int    center_element[],
                      const int    num_outer_centers,
                      const double outer_center_xyz[],
                      const int    outer_center_element[],
                      const int    num_shells,
                      const int    shell_center[],
                      const int    l_quantum_num[],
                      const int    shell_num_primitives[],
                      const double primitive_exp[]);

        int get_num_points() const;
        double *get_grid() const;

    private:

        Grid(const Grid &rhs);            // not implemented
        Grid &operator=(const Grid &rhs); // not implemented

        void nullify();

        int get_closest_num_angular(int n) const;
        int get_angular_order(int n) const;

        int num_points;
        double *xyzw;
};

#endif
