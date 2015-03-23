
#include <cstdlib>

#include "stdio.h"
#include "math.h"

#include "grid_radial.h"
#include "grid_constants.h"


// TCA 106, 178 (2001), eq. 25
// we evaluate r_inner for s functions
double get_r_inner(const double max_error,
                   const double alpha_inner)
{
    int m = 0;
    double d = 1.9;

    double r = d - log(1.0/max_error);
    r = r*2.0/(m + 3.0);
    r = exp(r)/(alpha_inner);
    r = sqrt(r);

    return r;
}


// TCA 106, 178 (2001), eq. 19
double get_r_outer(const double max_error,
                   const double alpha_outer,
                   const int    l,
                   const double guess)
{
    int    m = 2*l;
    double r = guess;
    double r_old = 1.0e50;
    double c, a, e;
    double step = 0.5;
    double sign, sign_old;
    double f = 1.0e50;

    (f > max_error) ? (sign = 1.0) : (sign = -1.0);

    while (fabs(r_old - r) > SMALL)
    {
        c = tgamma((m + 3.0)/2.0);
        a  = pow(alpha_outer*r*r, (m + 1.0)/2.0);
        e  = exp(-alpha_outer*r*r);
        f  = c*a*e;

        sign_old = sign;
        (f > max_error) ? (sign = 1.0) : (sign = -1.0);
        if (r < 0.0) sign = 1.0;
        if (sign != sign_old) step *= 0.1;

        r_old = r;
        r += sign*step;
    }

    return r;
}


// TCA 106, 178 (2001), eqs. 17 and 18
double get_h(const double max_error,
             const int    l,
             const double guess)
{
    int    m = 2*l;
    double h = guess;
    double h_old = 1.0e50;
    double step = 0.1*guess;
    double sign, sign_old;
    double f = 1.0e50;
    double c0, cm, p0, e0, pm, rd0, rdm;

    (f > max_error) ? (sign = -1.0) : (sign = 1.0);

    while (fabs(h_old - h) > SMALL)
    {
        c0  = 4.0*sqrt(2.0)*PI;
        cm  = tgamma(3.0/2.0)/tgamma((m + 3.0)/2.0);
        p0  = 1.0/h;
        e0  = exp(-PI*PI/(2.0*h));
        pm  = pow(PI/h, m/2.0);
        rd0 = c0*p0*e0;
        f   = cm*pm*rd0;

        sign_old = sign;
        (f > max_error) ? (sign = -1.0) : (sign = 1.0);
        if (h < 0.0) sign = 1.0;
        if (sign != sign_old) step *= 0.1;

        h_old = h;
        h += sign*step;
    }

    return h;
}
