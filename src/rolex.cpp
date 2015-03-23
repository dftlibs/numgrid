

#ifdef ENABLE_OMP
#include <omp.h>
#endif
#include <time.h>

#include "rolex.h"


namespace rolex
{
#ifdef ENABLE_OMP
    double  t_global, t_partial;
#else
    clock_t t_global, t_partial;
#endif


    void start_global()
    {
#ifdef ENABLE_OMP
        t_global = omp_get_wtime();
#else
        t_global = clock();
#endif
    }


    double stop_global()
    {
#ifdef ENABLE_OMP
        return omp_get_wtime() - t_global;
#else
        return (double)(clock() - t_global)/CLOCKS_PER_SEC;
#endif
    }


    void start_partial()
    {
#ifdef ENABLE_OMP
        t_partial = omp_get_wtime();
#else
        t_partial = clock();
#endif
    }


    double stop_partial()
    {
#ifdef ENABLE_OMP
        return omp_get_wtime() - t_partial;
#else
        return (double)(clock() - t_partial)/CLOCKS_PER_SEC;
#endif
    }
}
