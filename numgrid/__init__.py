import os
from cffi import FFI
from .cffi_helpers import get_lib_handle
import numpy as np


_this_path = os.path.dirname(os.path.realpath(__file__))

_build_dir = os.getenv('NUMGRID_BUILD_DIR')
if _build_dir is None:
    _build_dir = _this_path
else:
    _build_dir = os.path.join(_build_dir, 'lib')

_include_dir = _this_path

_lib = get_lib_handle(
    ['-DNUMGRID_API=', '-DCPP_INTERFACE_NOINCLUDE'],
    'numgrid.h',
    'numgrid',
    _build_dir,
    _include_dir
)

_ffi = FFI()

new_atom_grid = _lib.numgrid_new_atom_grid
get_num_grid_points = _lib.numgrid_get_num_grid_points
get_num_radial_grid_points = _lib.numgrid_get_num_radial_grid_points
free_atom_grid = _lib.numgrid_free_atom_grid


def get_version():
    return _ffi.string(_lib.numgrid_get_version()).decode('utf-8')


def get_grid(context,
             num_centers,
             i,
             x_coordinates_au,
             y_coordinates_au,
             z_coordinates_au,
             proton_charges):

    num_grid_points = get_num_grid_points(context)

    grid_x_au_np = np.zeros(num_grid_points, dtype=np.float64)
    grid_x_au_p = _ffi.cast("double *", grid_x_au_np.ctypes.data)
    grid_y_au_np = np.zeros(num_grid_points, dtype=np.float64)
    grid_y_au_p = _ffi.cast("double *", grid_y_au_np.ctypes.data)
    grid_z_au_np = np.zeros(num_grid_points, dtype=np.float64)
    grid_z_au_p = _ffi.cast("double *", grid_z_au_np.ctypes.data)
    grid_w_np = np.zeros(num_grid_points, dtype=np.float64)
    grid_w_p = _ffi.cast("double *", grid_w_np.ctypes.data)

    _lib.numgrid_get_grid(context,
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


def get_radial_grid(context):

    num_radial_points = get_num_radial_grid_points(context)

    radial_grid_r_au_np = np.zeros(num_radial_points, dtype=np.float64)
    radial_grid_r_au_p = _ffi.cast("double *", radial_grid_r_au_np.ctypes.data)
    radial_grid_w_np = np.zeros(num_radial_points, dtype=np.float64)
    radial_grid_w_p = _ffi.cast("double *", radial_grid_w_np.ctypes.data)

    _lib.numgrid_get_radial_grid(context,
                                 radial_grid_r_au_p,
                                 radial_grid_w_p)

    r = radial_grid_r_au_np.tolist()
    w = radial_grid_w_np.tolist()

    return r, w


def get_angular_grid(num_angular_grid_points):

    angular_grid_x_au_np = np.zeros(num_angular_grid_points, dtype=np.float64)
    angular_grid_x_au_p = _ffi.cast("double *", angular_grid_x_au_np.ctypes.data)
    angular_grid_y_au_np = np.zeros(num_angular_grid_points, dtype=np.float64)
    angular_grid_y_au_p = _ffi.cast("double *", angular_grid_y_au_np.ctypes.data)
    angular_grid_z_au_np = np.zeros(num_angular_grid_points, dtype=np.float64)
    angular_grid_z_au_p = _ffi.cast("double *", angular_grid_z_au_np.ctypes.data)
    angular_grid_w_np = np.zeros(num_angular_grid_points, dtype=np.float64)
    angular_grid_w_p = _ffi.cast("double *", angular_grid_w_np.ctypes.data)

    _lib.numgrid_get_angular_grid(num_angular_grid_points,
                                  angular_grid_x_au_p,
                                  angular_grid_y_au_p,
                                  angular_grid_z_au_p,
                                  angular_grid_w_p)

    x = angular_grid_x_au_np.tolist()
    y = angular_grid_y_au_np.tolist()
    z = angular_grid_z_au_np.tolist()
    w = angular_grid_w_np.tolist()

    return x, y, z, w
