#pragma once

#include <vector>

class AtomGrid
{
  public:
    AtomGrid(const double radial_precision,
             const int min_num_angular_points,
             const int max_num_angular_points,
             const int proton_charge,
             const double alpha_max,
             const int max_l_quantum_number,
             const double alpha_min[]);

    ~AtomGrid();

    int get_num_grid_points() const;

    int get_num_radial_grid_points() const;

    void get_grid(const int num_centers,
                  const int center_index,
                  const double x_coordinates_bohr[],
                  const double y_coordinates_bohr[],
                  const double z_coordinates_bohr[],
                  const int proton_charges[],
                  double grid_x_bohr[],
                  double grid_y_bohr[],
                  double grid_z_bohr[],
                  double grid_w[]) const;

    void get_radial_grid(double grid_r_bohr[], double grid_w[]) const;

  private:
    // TODO move to public section, use C++11, use delete
    AtomGrid(const AtomGrid &rhs);            // not implemented
    AtomGrid &operator=(const AtomGrid &rhs); // not implemented

    std::size_t num_grid_points_;

    std::vector<double> atom_grid_x_bohr_;
    std::vector<double> atom_grid_y_bohr_;
    std::vector<double> atom_grid_z_bohr_;
    std::vector<double> atom_grid_w_;

    std::size_t num_radial_grid_points_;

    std::vector<double> radial_atom_grid_r_bohr_;
    std::vector<double> radial_atom_grid_w_;
};
