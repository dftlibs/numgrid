#pragma once

double get_becke_w(const int num_centers,
                   const int proton_charges[],
                   const double x_coordinates_bohr[],
                   const double y_coordinates_bohr[],
                   const double z_coordinates_bohr[],
                   const int center_index,
                   const double x,
                   const double y,
                   const double z);
