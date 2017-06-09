/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED

class Grid
{
  public:
    Grid();
    ~Grid();

    int generate(const double radial_precision,
                 const int min_num_angular_points,
                 const int max_num_angular_points,
                 const int num_centers,
                 const double center_coordinates[],
                 const int center_elements[],
                 const int num_outer_centers,
                 const double outer_center_coordinates[],
                 const int outer_center_elements[],
                 const int num_shells,
                 const int shell_centers[],
                 const int shell_l_quantum_numbers[],
                 const int shell_num_primitives[],
                 const double primitive_exponents[]);

    int numgrid_get_num_points() const;
    const double * const numgrid_get_grid() const;

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
