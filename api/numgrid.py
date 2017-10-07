import sys
import os
from subprocess import Popen, PIPE
from cffi import FFI
import numpy as np


def _get_env(v):
    _v = os.getenv(v)
    if _v is None:
        sys.stderr.write('Error: Environment variable {0} is undefined\n'.format(v))
        sys.exit(1)
    return _v


def _get_library_suffix():
    if sys.platform == "darwin":
        return 'dylib'
    else:
        return 'so'


def _get_lib_handle(definitions, header, library, build_dir, include_dir):
    ffi = FFI()

    interface = Popen(['cc', '-E'] + definitions + [os.path.join(include_dir, header)],
                      stdout=PIPE).communicate()[0].decode("utf-8")
    ffi.cdef(interface)

    suffix = _get_library_suffix()
    lib = ffi.dlopen(os.path.join(build_dir, 'lib', 'lib{0}.{1}'.format(library, suffix)))
    return lib


_build_dir = _get_env('NUMGRID_BUILD_DIR')
_include_dir = os.path.dirname(os.path.realpath(__file__))

_lib = _get_lib_handle(
    ['-DNUMGRID_API=', '-DNUMGRID_NOINCLUDE'],
    'numgrid.h',
    'numgrid',
    _build_dir,
    _include_dir
)


_ffi = FFI()

def get_version():
    return _ffi.string(_lib.numgrid_get_version()).decode('utf-8')


new_atom_grid = _lib.numgrid_new_atom_grid
get_num_grid_points = _lib.numgrid_get_num_grid_points
free_atom_grid = _lib.numgrid_free_atom_grid


def get_grid_points(context,
                    num_centers,
                    i,
                    x_coordinates_au,
                    y_coordinates_au,
                    z_coordinates_au,
                    proton_charges):

    num_points = get_num_grid_points(context)

    grid_x_au_np = np.zeros(num_points, dtype=np.float64)
    grid_x_au_p = _ffi.cast("double *", grid_x_au_np.ctypes.data)
    grid_y_au_np = np.zeros(num_points, dtype=np.float64)
    grid_y_au_p = _ffi.cast("double *", grid_y_au_np.ctypes.data)
    grid_z_au_np = np.zeros(num_points, dtype=np.float64)
    grid_z_au_p = _ffi.cast("double *", grid_z_au_np.ctypes.data)
    grid_w_np = np.zeros(num_points, dtype=np.float64)
    grid_w_p = _ffi.cast("double *", grid_w_np.ctypes.data)

    _lib.numgrid_get_grid_points(context,
                                 num_centers,
                                 i,
                                 x_coordinates_au,
                                 y_coordinates_au,
                                 z_coordinates_au,
                                 proton_charges,
                                 grid_x_au_p,
                                 grid_y_au_p,
                                 grid_z_au_p,
                                 grid_w_p)

    x = grid_x_au_np.tolist()
    y = grid_y_au_np.tolist()
    z = grid_z_au_np.tolist()
    w = grid_w_np.tolist()

    return x, y, z, w
