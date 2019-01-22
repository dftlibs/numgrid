/* CFFI would issue warning with pragma once */
#ifndef NUMGRID_H_INCLUDED
#define NUMGRID_H_INCLUDED

#ifndef NUMGRID_API
#include "numgrid_export.h"
#define NUMGRID_API NUMGRID_EXPORT
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    struct context_s;
    typedef struct context_s context_t;

    /* get version */
    NUMGRID_API
    char *numgrid_get_version();

    /* create a new atom grid by providing explicit exponents*/
    NUMGRID_API
    context_t *numgrid_new_atom_grid(const double radial_precision,
                                     const int min_num_angular_points,
                                     const int max_num_angular_points,
                                     const int proton_charge,
                                     const double alpha_max,
                                     const int max_l_quantum_number,
                                     const double alpha_min[]);

    /* get number of grid points on current atom */
    NUMGRID_API
    int numgrid_get_num_grid_points(const context_t *context);

    /* get number of radial grid points on current atom */
    NUMGRID_API
    int numgrid_get_num_radial_grid_points(const context_t *context);

    /* get grid on current atom, scaled by becke partitioning */
    NUMGRID_API
    void numgrid_get_grid(const context_t *context,
                          const int num_centers,
                          const int center_index,
                          const double x_coordinates_bohr[],
                          const double y_coordinates_bohr[],
                          const double z_coordinates_bohr[],
                          const int proton_charges[],
                          double grid_x_bohr[],
                          double grid_y_bohr[],
                          double grid_z_bohr[],
                          double grid_w[]);

    /* get radial grid on current atom */
    NUMGRID_API
    void numgrid_get_radial_grid(const context_t *context,
                                 double radial_grid_r_bohr[],
                                 double radial_grid_w[]);

    /* get angular grid */
    NUMGRID_API
    void numgrid_get_angular_grid(const int num_angular_grid_points,
                                  double angular_grid_x_bohr[],
                                  double angular_grid_y_bohr[],
                                  double angular_grid_z_bohr[],
                                  double angular_grid_w[]);

    /* destroy the atom and deallocate all data */
    NUMGRID_API
    void numgrid_free_atom_grid(context_t *context);

#ifdef __cplusplus
}
#endif

#endif /* NUMGRID_H_INCLUDED */
