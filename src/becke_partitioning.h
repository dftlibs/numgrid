/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BECKE_PARTITIONING_H_INCLUDED
#define BECKE_PARTITIONING_H_INCLUDED

double get_becke_w(const double center_coordinates[],
                   const int center_charge[],
                   double pa[],
                   const int icent,
                   const int num_centers,
                   const double x,
                   const double y,
                   const double z);

double get_bragg_angstrom(const int charge);

#endif
