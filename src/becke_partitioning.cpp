#include <cstdlib>

#include "stdio.h"
#include "math.h"

#include "becke_partitioning.h"
#include "parameters.h"


// we use the same values as the DIRAC15 code
double get_bragg_angstrom(const int charge)
{
    if (charge ==   1) return 0.35;
    if (charge ==   2) return 0.35;
    if (charge ==   3) return 1.45;
    if (charge ==   4) return 1.05;
    if (charge ==   5) return 0.85;
    if (charge ==   6) return 0.70;
    if (charge ==   7) return 0.65;
    if (charge ==   8) return 0.60;
    if (charge ==   9) return 0.50;
    if (charge ==   0) return 0.45;
    if (charge ==  11) return 1.80;
    if (charge ==  12) return 1.50;
    if (charge ==  13) return 1.25;
    if (charge ==  14) return 1.10;
    if (charge ==  15) return 1.00;
    if (charge ==  16) return 1.00;
    if (charge ==  17) return 1.00;
    if (charge ==  18) return 1.00;
    if (charge ==  19) return 2.20;
    if (charge ==  20) return 1.80;
    if (charge ==  21) return 1.60;
    if (charge ==  22) return 1.40;
    if (charge ==  23) return 1.35;
    if (charge ==  24) return 1.40;
    if (charge ==  25) return 1.40;
    if (charge ==  26) return 1.40;
    if (charge ==  27) return 1.35;
    if (charge ==  28) return 1.35;
    if (charge ==  29) return 1.35;
    if (charge ==  30) return 1.35;
    if (charge ==  31) return 1.30;
    if (charge ==  32) return 1.25;
    if (charge ==  33) return 1.15;
    if (charge ==  34) return 1.15;
    if (charge ==  35) return 1.15;
    if (charge ==  36) return 1.10;
    if (charge ==  37) return 2.35;
    if (charge ==  38) return 2.00;
    if (charge ==  39) return 1.80;
    if (charge ==  40) return 1.55;
    if (charge ==  41) return 1.45;
    if (charge ==  42) return 1.45;
    if (charge ==  43) return 1.35;
    if (charge ==  44) return 1.30;
    if (charge ==  45) return 1.35;
    if (charge ==  46) return 1.40;
    if (charge ==  47) return 1.60;
    if (charge ==  48) return 1.55;
    if (charge ==  49) return 1.55;
    if (charge ==  50) return 1.45;
    if (charge ==  51) return 1.45;
    if (charge ==  52) return 1.40;
    if (charge ==  53) return 1.40;
    if (charge ==  54) return 1.40;
    if (charge ==  55) return 2.60;
    if (charge ==  56) return 2.15;
    if (charge ==  57) return 1.95;
    if (charge ==  58) return 1.85;
    if (charge ==  59) return 1.85;
    if (charge ==  60) return 1.85;
    if (charge ==  61) return 1.85;
    if (charge ==  62) return 1.85;
    if (charge ==  63) return 1.85;
    if (charge ==  64) return 1.80;
    if (charge ==  65) return 1.75;
    if (charge ==  66) return 1.75;
    if (charge ==  67) return 1.75;
    if (charge ==  68) return 1.75;
    if (charge ==  69) return 1.75;
    if (charge ==  70) return 1.75;
    if (charge ==  71) return 1.75;
    if (charge ==  72) return 1.55;
    if (charge ==  73) return 1.45;
    if (charge ==  74) return 1.35;
    if (charge ==  75) return 1.30;
    if (charge ==  76) return 1.30;
    if (charge ==  77) return 1.35;
    if (charge ==  78) return 1.35;
    if (charge ==  79) return 1.35;
    if (charge ==  80) return 1.50;
    if (charge ==  81) return 1.90;
    if (charge ==  82) return 1.75;
    if (charge ==  83) return 1.60;
    if (charge ==  84) return 1.90;
    if (charge ==  85) return 1.50;
    if (charge ==  86) return 1.50;

    fprintf(stderr, "ERROR: element not found in get_bragg_angstrom\n");
    exit(-1);
}


// JCP 88, 2547 (1988), eq. 20
double f3(const double x);
double f3(const double x)
{
    double f = x;
    for (int i = 0; i < BECKE_HARDNESS; i++)
    {
        f *= (1.5 - 0.5*f*f);
    }
    return f;
}


// JCP 88, 2547 (1988)
double get_becke_w(const double center_coordinates[],
                   const int    center_charge[],
                         double pa[],
                   const int    icent,
                   const int    num_centers,
                   const double x,
                   const double y,
                   const double z)
{
    double R_a, R_b;
    double u_ab, a_ab, mu_ab, nu_ab;
    double f;
    double dist_a, dist_b, dist_ab;
    double vx, vy, vz;

    for (int a = 0; a < num_centers; a++)
    {
        pa[a] = 1.0;
    }

    for (int a = 0; a < num_centers; a++)
    {
        vx = center_coordinates[a*3    ] - x;
        vy = center_coordinates[a*3 + 1] - y;
        vz = center_coordinates[a*3 + 2] - z;
        dist_a = vx*vx + vy*vy + vz*vz;
        dist_a = sqrt(dist_a);

        if (a != icent && dist_a > BECKE_CUTOFF)
        {
            pa[a] = 0.0;
            continue;
        }

        R_a = get_bragg_angstrom(center_charge[a]);

        for (int b = 0; b < a; b++)
        {
            if (a != b)
            {

                vx = center_coordinates[b*3    ] - x;
                vy = center_coordinates[b*3 + 1] - y;
                vz = center_coordinates[b*3 + 2] - z;
                dist_b = vx*vx + vy*vy + vz*vz;
                dist_b = sqrt(dist_b);

                R_b = get_bragg_angstrom(center_charge[b]);

                vx = center_coordinates[b*3    ] - center_coordinates[a*3    ];
                vy = center_coordinates[b*3 + 1] - center_coordinates[a*3 + 1];
                vz = center_coordinates[b*3 + 2] - center_coordinates[a*3 + 2];
                dist_ab = vx*vx + vy*vy + vz*vz;
                dist_ab = sqrt(dist_ab);

                // JCP 88, 2547 (1988), eq. 11
                mu_ab = (dist_a - dist_b)/dist_ab;

                if (fabs(R_a - R_b) > SMALL)
                {
                    u_ab = (R_a + R_b)/(R_b - R_a);
                    a_ab = u_ab/(u_ab*u_ab - 1.0);

                    // JCP 88, 2547 (1988), eq. A3
                    if (a_ab >  0.5) a_ab =  0.5;
                    if (a_ab < -0.5) a_ab = -0.5;

                    nu_ab = mu_ab + a_ab*(1.0 - mu_ab*mu_ab);
                }
                else
                {
                    nu_ab = mu_ab;
                }

                f = f3(nu_ab);

                if (fabs(1.0 - f) < SMALL)
                {
                    // if f == 1.0 we need to take care
                    // otherwise we can get numerical problems
                    pa[a] = 0.0;
                }
                else
                {
                    pa[a] *= 0.5*(1.0 - f);
                    pa[b] *= 0.5*(1.0 + f);
                }
            }
        }
    }

    double w = 0.0;
    for (int a = 0; a < num_centers; a++)
    {
        w += pa[a];
    }

    if (fabs(w) > SMALL)
    {
        return pa[icent]/w;
    }
    else
    {
        return 1.0;
    }
}
