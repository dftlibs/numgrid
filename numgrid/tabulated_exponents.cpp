#include "tabulated_exponents.h"
#include "error_handling.h"

std::vector<double> alpha_min_tabulated(const int charge)
{
    switch (charge)
    {
    case 1:
        return {0.122, 0.727};
    case 8:
        return {0.3023, 0.2753, 1.185};
    default:
        NUMGRID_ERROR("No entry found in alpha_min_tabulated");
    }
    return {0.0};
}

double alpha_max_tabulated(const int charge)
{
    switch (charge)
    {
    case 1:
        return 13.01;
    case 8:
        return 11720.0;
    default:
        NUMGRID_ERROR("No entry found in alpha_max_tabulated");
    }
    return 0.0;
}

int max_l_quantum_number_tabulated(const int charge)
{
    switch (charge)
    {
    case 1:
        return 1;
    case 8:
        return 2;
    default:
        NUMGRID_ERROR("No entry found in max_l_quantum_number_tabulated");
    }
    return 0;
}
