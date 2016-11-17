#ifndef NUMGRID_H_INCLUDED
#define NUMGRID_H_INCLUDED

#ifndef NUMGRID_API
#  ifdef _WIN32
#    if defined(NUMGRID_BUILD_SHARED) /* build dll */
#      define NUMGRID_API __declspec(dllexport)
#    elif !defined(NUMGRID_BUILD_STATIC) /* use dll */
#      define NUMGRID_API __declspec(dllimport)
#    else /* static library */
#      define NUMGRID_API
#    endif
#  else
#    if __GNUC__ >= 4
#      define NUMGRID_API __attribute__((visibility("default")))
#    else
#      define NUMGRID_API
#    endif
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct numgrid_context_s;
typedef struct numgrid_context_s numgrid_context_t;

NUMGRID_API numgrid_context_t *numgrid_new();
NUMGRID_API void numgrid_free(numgrid_context_t *context);

NUMGRID_API int numgrid_generate(numgrid_context_t *context,
                                 const double radial_precision,
                                 const int    min_num_angular_points,
                                 const int    max_num_angular_points,
                                 const int    num_centers,
                                 const double center_coordinates[],
                                 const int    center_elements[],
                                 const int    num_outer_centers,
                                 const double outer_center_coordinates[],
                                 const int    outer_center_elements[],
                                 const int    num_shells,
                                 const int    shell_centers[],
                                 const int    shell_l_quantum_numbers[],
                                 const int    shell_num_primitives[],
                                 const double primitive_exponents[]);

NUMGRID_API int numgrid_get_num_points(const numgrid_context_t *context);
NUMGRID_API double *numgrid_get_grid(const numgrid_context_t *context);

#ifdef __cplusplus
}
#endif

#endif /* NUMGRID_H_INCLUDED */
