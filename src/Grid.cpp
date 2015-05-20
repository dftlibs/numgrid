
// must be included before fstream (stdio)
#include "Grid.h"

#include <cstddef>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

#include "math.h"

#include "io.h"
#include "rolex.h"
#include "parameters.h"
#include "sphere_lebedev_rule.h"
#include "grid_radial.h"
#include "becke_partitioning.h"
#include "MemAllocator.h"

#include "numgrid.h"

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)


numgrid_context_t *numgrid_new()
{
    return AS_TYPE(numgrid_context_t, new Grid());
}
Grid::Grid()
{
    nullify();
}


void numgrid_free(numgrid_context_t *context)
{
    if (!context) return;
    delete AS_TYPE(Grid, context);
}
Grid::~Grid()
{
    MemAllocator::deallocate(pw);

    nullify();
}


int numgrid_get_num_points(const numgrid_context_t *context)
{
    return AS_CTYPE(Grid, context)->num_points;
}


double *numgrid_get_grid(const numgrid_context_t *context)
{
    return AS_CTYPE(Grid, context)->pw;
}


void Grid::nullify()
{
    pw = NULL;
    num_points = -1;
    is_generated = false;
    is_distributed = false;
}


int lebedev_table[33] = {6,   14,   26,   38,   50,   74,   86,  110,  146,  170,
                       194,  230,  266,  302,  350,  434,  590,  770,  974, 1202,
                      1454, 1730, 2030, 2354, 2702, 3074, 3470, 3890, 4334, 4802,
                      4934, 5294, 5810};


int Grid::get_closest_num_angular(int n)
{
    int m;

    for (int i = 0; i < MAX_ANGULAR_ORDER; i++)
    {
        m = lebedev_table[i];
        if (m >= n) return m;
    }

    fprintf(stderr, "ERROR: input n too high in get_closest_num_angular\n");
    exit(-1);
}


int Grid::get_angular_order(int n)
{
    for (int i = 0; i < MAX_ANGULAR_ORDER; i++)
    {
        if (lebedev_table[i] == n) return i;
    }

    fprintf(stderr, "ERROR: no match found in get_angular_offset\n");
    exit(-1);
}


void Grid::generate_sub(const int    num_centers,
                        const double center_xyz[],
                        const int    center_element[],
                        const int    num_shells,
                        const int    shell_center[],
                        const int    l_quantum_num[],
                        const int    shell_num_primitives[],
                        const double primitive_exp[],
                        const int    num_angular_min,
                        const int    num_angular_max,
                        const double angular_x[],
                        const double angular_y[],
                        const double angular_z[],
                        const double angular_w[])
{
    int *num_points_on_atom = (int*) MemAllocator::allocate(num_centers*sizeof(int));

    // first round figures out dimensions
    // second round allocates and does the real work
    for (int iround = 0; iround < 2; iround++)
    {
            for (int icent = 0; icent < num_centers; icent++)
            {
                double *pa_buffer = (double*) MemAllocator::allocate(num_centers*sizeof(double));

                // get extreme alpha values
                double alpha_max = 0.0;
                int l_max = 0;
                double alpha_min[MAX_L_VALUE + 1];
                bool   alpha_min_set[MAX_L_VALUE + 1];
                std::fill(&alpha_min[0],     &alpha_min[MAX_L_VALUE + 1],     0.0);
                std::fill(&alpha_min_set[0], &alpha_min_set[MAX_L_VALUE + 1], false);

                int n = 0;
                for (int ishell = 0; ishell < num_shells; ishell++)
                {
                    if ((shell_center[ishell] - 1) == icent)
                    {
                        int l = l_quantum_num[ishell];
                        l_max = std::max(l_max, l);

                        if (!alpha_min_set[l])
                        {
                            alpha_min[l] = 1.0e50;
                            alpha_min_set[l] = true;
                        }

                        for (int p = 0; p < shell_num_primitives[ishell]; p++)
                        {
                            double e = primitive_exp[n];
                            alpha_max    = std::max(alpha_max, 2.0*e); // factor 2.0 to match DIRAC
                            alpha_min[l] = std::min(alpha_min[l], e);
                        }
                    }
                    n += shell_num_primitives[ishell];
                }

                // obtain radial parameters
                double r_inner = get_r_inner(radial_precision, alpha_max);
                double h       = 1.0e50;
                double r_outer = 0.0;
                for (int l = 0; l <= l_max; l++)
                {
                    if (alpha_min[l] > 0.0)
                    {
                        r_outer = std::max(r_outer, get_r_outer(radial_precision, alpha_min[l], l, 4.0*get_bragg_angstrom(center_element[icent])));
                        assert(r_outer > r_inner);
                        h = std::min(h, get_h(radial_precision, l, 0.1*(r_outer - r_inner)));
                    }
                }
                assert(r_outer > h);

                int ioff = 0;

                if (iround == 0)
                {
                    num_points_on_atom[icent] = 0;
                }
                else
                {
                    for (int jcent = 0; jcent < icent; jcent++)
                    {
                        ioff += num_points_on_atom[jcent];
                    }
                }

                double rb = get_bragg_angstrom(center_element[icent])/(5.0*0.529177249); // factors match DIRAC code
                double c = r_inner/(exp(h) - 1.0);
                int num_radial = int(log(1.0 + (r_outer/c))/h);
                for (int irad = 0; irad < num_radial; irad++)
                {
                    double radial_r = c*(exp((irad+1)*h) - 1.0);
                    double radial_w = (radial_r + c)*radial_r*radial_r*h;

                    int num_angular = num_angular_max;
                    if (radial_r < rb)
                    {
                        num_angular = static_cast<int>(num_angular_max*(radial_r/rb));
                        num_angular = get_closest_num_angular(num_angular);
                        if (num_angular < num_angular_min) num_angular = num_angular_min;
                    }

                    if (iround == 0)
                    {
                        num_points_on_atom[icent] += num_angular;
                    }
                    else
                    {
                        int angular_off = get_angular_order(num_angular)*MAX_ANGULAR_GRID;

                        for (int iang = 0; iang < num_angular; iang++)
                        {
                            pw[4*(ioff + iang)    ] = center_xyz[icent*3    ] + angular_x[angular_off + iang]*radial_r;
                            pw[4*(ioff + iang) + 1] = center_xyz[icent*3 + 1] + angular_y[angular_off + iang]*radial_r;
                            pw[4*(ioff + iang) + 2] = center_xyz[icent*3 + 2] + angular_z[angular_off + iang]*radial_r;

                            double becke_w = 1.0;
                            if (num_centers > 1)
                            {
                                becke_w = get_becke_w(center_xyz,
                                                      center_element,
                                                      pa_buffer,
                                                      icent,
                                                      num_centers,
                                                      pw[4*(ioff + iang)    ],
                                                      pw[4*(ioff + iang) + 1],
                                                      pw[4*(ioff + iang) + 2]);
                            }

                            pw[4*(ioff + iang) + 3] = 4.0*PI*angular_w[angular_off + iang]*radial_w*becke_w;
                        }
                    }

                    ioff += num_angular;
                }

                MemAllocator::deallocate(pa_buffer);
            }

        if (iround == 0)
        {
            num_points = 0;
            for (int icent = 0; icent < num_centers; icent++)
            {
                num_points += num_points_on_atom[icent];
            }

            pw = (double*) MemAllocator::allocate(4*num_points*sizeof(double));
        }
    }

    MemAllocator::deallocate(num_points_on_atom);
}


void numgrid_generate(numgrid_context_t *context,
                      const double radial_precision,
                      const int    angular_min,
                      const int    angular_max,
                      const int    verbosity,
                      const int    num_centers,
                      const double center_xyz[],
                      const int    center_element[],
                      const int    num_shells,
                      const int    shell_center[],
                      const int    l_quantum_num[],
                      const int    shell_num_primitives[],
                      const double primitive_exp[])
{
    return AS_TYPE(Grid, context)->generate(radial_precision,
                                            angular_min,
                                            angular_max,
                                            verbosity,
                                            num_centers,
                                            center_xyz,
                                            center_element,
                                            num_shells,
                                            shell_center,
                                            l_quantum_num,
                                            shell_num_primitives,
                                            primitive_exp);
}
void Grid::generate(const double in_radial_precision,
                    const int    in_angular_min,
                    const int    in_angular_max,
                    const int    verbosity,
                    const int    num_centers,
                    const double center_xyz[],
                    const int    center_element[],
                    const int    num_shells,
                    const int    shell_center[],
                    const int    l_quantum_num[],
                    const int    shell_num_primitives[],
                    const double primitive_exp[])
{
    radial_precision = in_radial_precision;
    angular_min      = in_angular_min;
    angular_max      = in_angular_max;

    if (is_generated) return;

    if (verbosity > 0)
    {
        io::speak_your_mind("\n\nXCint grid\n");
        io::speak_your_mind("----------\n\n");
    }

    rolex::start_partial();
    if (verbosity > 0) io::speak_your_mind("Starting generating grid ...\n");

    int num_angular_min = get_closest_num_angular(angular_min);
    int num_angular_max = get_closest_num_angular(angular_max);
    if (verbosity > 0) io::speak_your_mind("Angular grid range: %i-%i\n", num_angular_min, num_angular_max);

    double *angular_x = (double*) MemAllocator::allocate(MAX_ANGULAR_ORDER*MAX_ANGULAR_GRID*sizeof(double));
    double *angular_y = (double*) MemAllocator::allocate(MAX_ANGULAR_ORDER*MAX_ANGULAR_GRID*sizeof(double));
    double *angular_z = (double*) MemAllocator::allocate(MAX_ANGULAR_ORDER*MAX_ANGULAR_GRID*sizeof(double));
    double *angular_w = (double*) MemAllocator::allocate(MAX_ANGULAR_ORDER*MAX_ANGULAR_GRID*sizeof(double));

    for (int i = get_angular_order(num_angular_min); i <= get_angular_order(num_angular_max); i++)
    {
        int angular_off = i*MAX_ANGULAR_GRID;
        ld_by_order(lebedev_table[i], &angular_x[angular_off], &angular_y[angular_off], &angular_z[angular_off], &angular_w[angular_off]);
    }

    generate_sub(num_centers,
                 center_xyz,
                 center_element,
                 num_shells,
                 shell_center,
                 l_quantum_num,
                 shell_num_primitives,
                 primitive_exp,
                 num_angular_min,
                 num_angular_max,
                 angular_x,
                 angular_y,
                 angular_z,
                 angular_w);

    MemAllocator::deallocate(angular_x);
    MemAllocator::deallocate(angular_y);
    MemAllocator::deallocate(angular_z);
    MemAllocator::deallocate(angular_w);

    if (verbosity > 0) io::speak_your_mind("Finished generating grid after %.2f seconds.\n", rolex::stop_partial());
    if (verbosity > 0) io::speak_your_mind("Total number of points: %i\n", num_points);

    is_generated = true;
}
