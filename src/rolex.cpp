

#include <time.h>

#include "rolex.h"


namespace rolex
{
    clock_t t_global, t_partial;


    void start_global()
    {
        t_global = clock();
    }


    double stop_global()
    {
        return (double)(clock() - t_global)/CLOCKS_PER_SEC;
    }


    void start_partial()
    {
        t_partial = clock();
    }


    double stop_partial()
    {
        return (double)(clock() - t_partial)/CLOCKS_PER_SEC;
    }
}
