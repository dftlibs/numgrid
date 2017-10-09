#pragma once

double get_r_inner(const double max_error, const double alpha_inner);

double get_r_outer(const double max_error,
                   const double alpha_outer,
                   const int l,
                   const double guess);

double get_h(const double max_error, const int l, const double guess);
