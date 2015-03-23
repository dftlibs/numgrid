
#include <cstddef>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

#include "math.h"

#include "Grid.h"

#include "io.h"
#include "rolex.h"
#include "parameters.h"
#include "sphere_lebedev_rule.h"
#include "grid_constants.h"
#include "grid_radial.h"
#include "becke_partitioning.h"
#include "MemAllocator.h"


const int    max_angular_order        = 33;
const int    max_angular_grid         = 5810;
const double default_radial_precision = 1.0e-12;
const int    default_angular_min      = 86;
const int    default_angular_max      = 302;


Grid::Grid()
{
    nullify();
    radial_precision = default_radial_precision;
    angular_min      = default_angular_min;
    angular_max      = default_angular_max;
}


Grid::~Grid()
{
    MemAllocator::deallocate(p);
    MemAllocator::deallocate(w);

    nullify();
}


void Grid::nullify()
{
    p = NULL;
    w = NULL;
    num_points = -1;
    is_generated = false;
    is_distributed = false;
}


void Grid::stretch()
// stretch grid so that it aligns nicely with block length
// if already aligned, we do nothing
// redefines num_points and reallocates p and w
{
    if (num_points%AO_BLOCK_LENGTH > 0)
    {
        int num_points_aligned = AO_BLOCK_LENGTH*(1 + num_points/AO_BLOCK_LENGTH);

        double *p_temp;
        double *w_temp;
        size_t block_size = num_points*sizeof(double);
        p_temp = (double*) MemAllocator::allocate(3*block_size);
        w_temp = (double*) MemAllocator::allocate(block_size);

        std::copy(p, p + 3*num_points, &p_temp[0]);
        std::copy(w, w +   num_points, &w_temp[0]);

        MemAllocator::deallocate(p);
        MemAllocator::deallocate(w);

        block_size = num_points_aligned*sizeof(double);
        p = (double*) MemAllocator::allocate(3*block_size);
        w = (double*) MemAllocator::allocate(block_size);

        std::fill(&p[0], &p[3*num_points_aligned], 1.0e10);
        std::fill(&w[0], &w[  num_points_aligned], 0.0);

        std::copy(p_temp, p_temp + 3*num_points, &p[0]);
        std::copy(w_temp, w_temp +   num_points, &w[0]);

        MemAllocator::deallocate(p_temp);
        MemAllocator::deallocate(w_temp);

        num_points = num_points_aligned;
    }
}


int lebedev_table[33] = {6,   14,   26,   38,   50,   74,   86,  110,  146,  170,
                       194,  230,  266,  302,  350,  434,  590,  770,  974, 1202,
                      1454, 1730, 2030, 2354, 2702, 3074, 3470, 3890, 4334, 4802,
                      4934, 5294, 5810};


int Grid::get_closest_num_angular(int n)
{
    int m;

    for (int i = 0; i < max_angular_order; i++)
    {
        m = lebedev_table[i];
        if (m >= n) return m;
    }

    fprintf(stderr, "ERROR: input n too high in get_closest_num_angular\n");
    exit(-1);
}


int Grid::get_angular_order(int n)
{
    for (int i = 0; i < max_angular_order; i++)
    {
        if (lebedev_table[i] == n) return i;
    }

    fprintf(stderr, "ERROR: no match found in get_angular_offset\n");
    exit(-1);
}


void Grid::set_grid_parameters(const double in_radial_precision,
                               const int    in_angular_min,
                               const int    in_angular_max)
{
    radial_precision = in_radial_precision;
    angular_min      = in_angular_min;
    angular_max      = in_angular_max;
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
#ifdef ENABLE_OMP
        #pragma omp parallel
        {
            #pragma omp for schedule(dynamic,1)
#endif
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
                        int angular_off = get_angular_order(num_angular)*max_angular_grid;

                        for (int iang = 0; iang < num_angular; iang++)
                        {
                            p[3*(ioff + iang)    ] = center_xyz[icent*3    ] + angular_x[angular_off + iang]*radial_r;
                            p[3*(ioff + iang) + 1] = center_xyz[icent*3 + 1] + angular_y[angular_off + iang]*radial_r;
                            p[3*(ioff + iang) + 2] = center_xyz[icent*3 + 2] + angular_z[angular_off + iang]*radial_r;

                            double becke_w = 1.0;
                            if (num_centers > 1)
                            {
                                becke_w = get_becke_w(center_xyz,
                                                      center_element,
                                                      pa_buffer,
                                                      icent,
                                                      num_centers,
                                                      p[3*(ioff + iang)    ],
                                                      p[3*(ioff + iang) + 1],
                                                      p[3*(ioff + iang) + 2]);
                            }

                            w[ioff + iang] = 4.0*PI*angular_w[angular_off + iang]*radial_w*becke_w;
                        }
                    }

                    ioff += num_angular;
                }

                MemAllocator::deallocate(pa_buffer);
            }
#ifdef ENABLE_OMP
        }
#endif

        if (iround == 0)
        {
            num_points = 0;
            for (int icent = 0; icent < num_centers; icent++)
            {
                num_points += num_points_on_atom[icent];
            }

            p = (double*) MemAllocator::allocate(3*num_points*sizeof(double));
            w = (double*) MemAllocator::allocate(num_points*sizeof(double));
        }
    }

    MemAllocator::deallocate(num_points_on_atom);
}


void Grid::generate(const int    verbosity,
                    const int    num_centers,
                    const double center_xyz[],
                    const int    center_element[],
                    const int    num_shells,
                    const int    shell_center[],
                    const int    l_quantum_num[],
                    const int    shell_num_primitives[],
                    const double primitive_exp[])
{
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

    double *angular_x = (double*) MemAllocator::allocate(max_angular_order*max_angular_grid*sizeof(double));
    double *angular_y = (double*) MemAllocator::allocate(max_angular_order*max_angular_grid*sizeof(double));
    double *angular_z = (double*) MemAllocator::allocate(max_angular_order*max_angular_grid*sizeof(double));
    double *angular_w = (double*) MemAllocator::allocate(max_angular_order*max_angular_grid*sizeof(double));

    for (int i = get_angular_order(num_angular_min); i <= get_angular_order(num_angular_max); i++)
    {
        int angular_off = i*max_angular_grid;
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

    // FIXME not correct since we may stretch to align on mem boundaries
    if (verbosity > 0) io::speak_your_mind("Total number of points: %i\n", num_points);

    is_generated = true;
}


void Grid::read()
{
    if (is_generated) return;

    int num_batches;
    int num_points_batch;

    std::ifstream infile("num_grid");

    // first we get the total number of points
    num_points = 0;
    double foo;
    infile >> num_batches;
    for (int ibatch = 0; ibatch < num_batches; ibatch++)
    {
        infile >> num_points_batch;
        num_points += num_points_batch;
        for (int ipoint = 0; ipoint < num_points_batch; ipoint++)
        {
            for (int k = 0; k < 3; k++)
            {
               infile >> foo;
            }
            infile >> foo;
        }
    }

    // free just in case
    MemAllocator::deallocate(p);
    MemAllocator::deallocate(w);

    // allocate
    size_t block_size = num_points*sizeof(double);
    p = (double*) MemAllocator::allocate(3*block_size);
    w = (double*) MemAllocator::allocate(block_size);

    // now we read for real
    infile.seekg(0); // rewind
    infile >> num_batches;
    int np = 0;
    int nw = 0;
    for (int ibatch = 0; ibatch < num_batches; ibatch++)
    {
        infile >> num_points_batch;
        for (int ipoint = 0; ipoint < num_points_batch; ipoint++)
        {
            for (int k = 0; k < 3; k++)
            {
               infile >> p[np++];
            }
            infile >> w[nw++];
        }
    }

    infile.close();

    is_generated = true;
}


#ifdef ENABLE_MPI
void Grid::distribute(const MPI_Comm &comm)
// redefines num_points
{
    if (is_distributed) return;

    int rank = 0;
    int num_proc = 1;
    size_t block_size;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &num_proc);

    MPI_Bcast(&num_points, 1, MPI_INT, 0, comm);

    int num_points_proc = num_points/num_proc;
    int num_points_rest = num_points%num_proc;

    if (rank != 0)
    {
        // free just in case
        MemAllocator::deallocate(p);
        MemAllocator::deallocate(w);

        block_size = num_points_proc*sizeof(double);
        p = (double*) MemAllocator::allocate(3*block_size);
        w = (double*) MemAllocator::allocate(block_size);
    }

    if (rank == 0)
    {
        double *buffer = NULL;
        block_size = 3*num_points_proc*sizeof(double);
        buffer = (double*) MemAllocator::allocate(block_size);

        MPI_Scatter(p, 3*num_points_proc, MPI_DOUBLE, buffer, 3*num_points_proc, MPI_DOUBLE, 0, comm);
        MPI_Scatter(w,   num_points_proc, MPI_DOUBLE, buffer,   num_points_proc, MPI_DOUBLE, 0, comm);

        MemAllocator::deallocate(buffer);
    }
    else
    {
        double empty[1] = {0.0};
        MPI_Scatter(empty, 0, MPI_DOUBLE, p, 3*num_points_proc, MPI_DOUBLE, 0, comm);
        MPI_Scatter(empty, 0, MPI_DOUBLE, w,   num_points_proc, MPI_DOUBLE, 0, comm);
    }

    // if there is a rest of points, master will do them
    if (num_points_rest > 0 && rank == 0)
    {
        int n = num_points_proc;
        for (int i = num_points-num_points_rest; i < num_points; i++)
        {
            w[n]       = w[i];
            p[3*n]     = p[3*i];
            p[3*n + 1] = p[3*i + 1];
            p[3*n + 2] = p[3*i + 2];
            n++;
        }
        num_points_proc += num_points_rest;
    }

    // verify that sum matches num_points_batch
    int control_sum = num_points_proc;

    if (rank == 0)
    {
        MPI_Reduce(MPI_IN_PLACE, &control_sum, 1, MPI_INT, MPI_SUM, 0, comm);
        assert(control_sum == num_points);
    }
    else
    {
        MPI_Reduce(&control_sum, MPI_IN_PLACE, 1, MPI_INT, MPI_SUM, 0, comm);
    }

    num_points = num_points_proc;

    is_distributed = true;
}
#endif
