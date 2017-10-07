#ifndef NUMGRID_H_INCLUDED
#define NUMGRID_H_INCLUDED

#ifndef NUMGRID_API
#include "numgrid_export.h"
#define NUMGRID_API numgrid_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct context_s;
typedef struct context_s context_t;

/* get version */
NUMGRID_API
char *numgrid_get_version();

/* create a new atom grid */
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

/* get grid on current atom, scaled by becke partitioning */
NUMGRID_API
void numgrid_get_grid_points(const context_t *context,
                             const int num_centers,
                             const int center_index,
                             const double x_coordinates_au[],
                             const double y_coordinates_au[],
                             const double z_coordinates_au[],
                             const int proton_charges[],
                             double grid_x_au[],
                             double grid_y_au[],
                             double grid_z_au[],
                             double grid_w[]);

/* destroy the atom and deallocate all data */
NUMGRID_API
void numgrid_free_atom_grid(context_t *context);

#ifdef __cplusplus
}
#endif

#endif /* NUMGRID_H_INCLUDED */
