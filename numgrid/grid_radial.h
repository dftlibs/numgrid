/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GRID_RADIAL_H_INCLUDED
#define GRID_RADIAL_H_INCLUDED

double get_r_inner(const double max_error, const double alpha_inner);

double get_r_outer(const double max_error,
                   const double alpha_outer,
                   const int l,
                   const double guess);

double get_h(const double max_error, const int l, const double guess);

#endif
