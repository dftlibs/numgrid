#ifndef BECKE_PARTITIONING_H_INCLUDED
#define BECKE_PARTITIONING_H_INCLUDED

double get_becke_w(const double center_xyz[],
                   const int    center_charge[],
                         double pa[],
                   const int    icent,
                   const int    num_centers,
                   const double x,
                   const double y,
                   const double z);

double get_bragg_angstrom(const int charge);

#endif
