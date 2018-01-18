/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <cmath>

#include "becke_partitioning.h"
#include "bragg.h"
#include "error_handling.h"
#include "parameters.h"

// JCP 88, 2547 (1988), eq. 20
inline double f3(const double x)
{
    double f = x;
    for (int i = 0; i < BECKE_HARDNESS; i++)
    {
        f *= (1.5 - 0.5 * f * f);
    }
    return f;
}

// JCP 88, 2547 (1988)
double get_becke_w(const int num_centers,
                   const int proton_charges[],
                   const double x_coordinates_bohr[],
                   const double y_coordinates_bohr[],
                   const double z_coordinates_bohr[],
                   const int center_index,
                   const double x,
                   const double y,
                   const double z)
{
    double R_a, R_b;
    double u_ab, a_ab, mu_ab, nu_ab;
    double f;
    double dist_a, dist_b, dist_ab;
    double vx, vy, vz;

    double *pa = new double[num_centers];

    for (int a = 0; a < num_centers; a++)
    {
        pa[a] = 1.0;
    }

    for (int a = 0; a < num_centers; a++)
    {
        vx = x_coordinates_bohr[a] - x;
        vy = y_coordinates_bohr[a] - y;
        vz = z_coordinates_bohr[a] - z;
        dist_a = vx * vx + vy * vy + vz * vz;
        dist_a = std::sqrt(dist_a);

        //      in principle good idea but fails for larger molecules containing
        //      diffuse sets if (a != icent && dist_a > BECKE_CUTOFF)
        //      {
        //          pa[a] = 0.0;
        //          continue;
        //      }

        R_a = get_bragg_angstrom(proton_charges[a]);

        for (int b = 0; b < a; b++)
        {
            if (a != b)
            {

                vx = x_coordinates_bohr[b] - x;
                vy = y_coordinates_bohr[b] - y;
                vz = z_coordinates_bohr[b] - z;
                dist_b = vx * vx + vy * vy + vz * vz;
                dist_b = std::sqrt(dist_b);

                R_b = get_bragg_angstrom(proton_charges[b]);

                vx = x_coordinates_bohr[b] - x_coordinates_bohr[a];
                vy = y_coordinates_bohr[b] - y_coordinates_bohr[a];
                vz = z_coordinates_bohr[b] - z_coordinates_bohr[a];
                dist_ab = vx * vx + vy * vy + vz * vz;
                dist_ab = std::sqrt(dist_ab);

                // JCP 88, 2547 (1988), eq. 11
                mu_ab = (dist_a - dist_b) / dist_ab;

                if (std::fabs(R_a - R_b) > SMALL)
                {
                    u_ab = (R_a + R_b) / (R_b - R_a);
                    a_ab = u_ab / (u_ab * u_ab - 1.0);

                    // JCP 88, 2547 (1988), eq. A3
                    if (a_ab > 0.5)
                        a_ab = 0.5;
                    if (a_ab < -0.5)
                        a_ab = -0.5;

                    nu_ab = mu_ab + a_ab * (1.0 - mu_ab * mu_ab);
                }
                else
                {
                    nu_ab = mu_ab;
                }

                f = f3(nu_ab);

                if (std::fabs(1.0 - f) < SMALL)
                {
                    // if f == 1.0 we need to take care
                    // otherwise we can get numerical problems
                    pa[a] = 0.0;
                }
                else
                {
                    pa[a] *= 0.5 * (1.0 - f);
                    pa[b] *= 0.5 * (1.0 + f);
                }
            }
        }
    }

    double w = 0.0;
    for (int a = 0; a < num_centers; a++)
    {
        w += pa[a];
    }

    double res = 1.0;
    if (std::fabs(w) > SMALL)
    {
        res = pa[center_index] / w;
    }

    delete[] pa;

    return res;
}
