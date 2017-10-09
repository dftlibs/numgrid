#ifndef BECKE_PARTITIONING_H_INCLUDED
#define BECKE_PARTITIONING_H_INCLUDED

double get_becke_w(const int num_centers,
                   const int proton_charges[],
                   const double x_coordinates_au[],
                   const double y_coordinates_au[],
                   const double z_coordinates_au[],
                   const int center_index,
                   const double x,
                   const double y,
                   const double z);

#endif /* BECKE_PARTITIONING_H_INCLUDED */