from subprocess import check_output
from cffi import FFI
import os
import sys
from configparser import ConfigParser
from pathlib import Path
import numpy as np


def get_lib_handle(definitions, header_file, library_file):
    ffi = FFI()
    command = ['cc', '-E'] + definitions + [header_file]
    interface = check_output(command).decode('utf-8')

    # remove possible \r characters on windows which
    # would confuse cdef
    _interface = [l.strip('\r') for l in interface.split('\n')]

    ffi.cdef('\n'.join(_interface))
    lib = ffi.dlopen(library_file)
    return lib


# this interface requires the header file and library file
# and these can be either provided by interface_file_names.cfg
# in the same path as this file
# or if this is not found then using environment variables
_this_path = Path(os.path.dirname(os.path.realpath(__file__)))
_cfg_file = _this_path / 'interface_file_names.cfg'
if _cfg_file.exists():
    config = ConfigParser()
    config.read(_cfg_file)
    header_file_name = config.get('configuration', 'header_file_name')
    _header_file = _this_path / 'include' /  header_file_name
    _header_file = str(_header_file)
    library_file_name = config.get('configuration', 'library_file_name')
    _library_file = _this_path / 'lib' / library_file_name
    _library_file = str(_library_file)
else:
    _header_file = os.getenv('NUMGRID_HEADER_FILE')
    assert _header_file is not None
    _library_file = os.getenv('NUMGRID_LIBRARY_FILE')
    assert _library_file is not None

_lib = get_lib_handle(definitions=['-DNUMGRID_API=', '-DNUMGRID_NOINCLUDE'],
                      header_file=_header_file,
                      library_file=_library_file)

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
             x_coordinates_bohr,
             y_coordinates_bohr,
             z_coordinates_bohr,
             proton_charges):

    num_grid_points = get_num_grid_points(context)

    grid_x_bohr_np = np.zeros(num_grid_points, dtype=np.float64)
    grid_x_bohr_p = _ffi.cast("double *", grid_x_bohr_np.ctypes.data)
    grid_y_bohr_np = np.zeros(num_grid_points, dtype=np.float64)
    grid_y_bohr_p = _ffi.cast("double *", grid_y_bohr_np.ctypes.data)
    grid_z_bohr_np = np.zeros(num_grid_points, dtype=np.float64)
    grid_z_bohr_p = _ffi.cast("double *", grid_z_bohr_np.ctypes.data)
    grid_w_np = np.zeros(num_grid_points, dtype=np.float64)
    grid_w_p = _ffi.cast("double *", grid_w_np.ctypes.data)

    _lib.numgrid_get_grid(context,
                          num_centers,
                          i,
                          x_coordinates_bohr,
                          y_coordinates_bohr,
                          z_coordinates_bohr,
                          proton_charges,
                          grid_x_bohr_p,
                          grid_y_bohr_p,
                          grid_z_bohr_p,
                          grid_w_p)

    x = grid_x_bohr_np.tolist()
    y = grid_y_bohr_np.tolist()
    z = grid_z_bohr_np.tolist()
    w = grid_w_np.tolist()

    return x, y, z, w


def get_radial_grid(context):

    num_radial_points = get_num_radial_grid_points(context)

    radial_grid_r_bohr_np = np.zeros(num_radial_points, dtype=np.float64)
    radial_grid_r_bohr_p = _ffi.cast("double *", radial_grid_r_bohr_np.ctypes.data)
    radial_grid_w_np = np.zeros(num_radial_points, dtype=np.float64)
    radial_grid_w_p = _ffi.cast("double *", radial_grid_w_np.ctypes.data)

    _lib.numgrid_get_radial_grid(context,
                                 radial_grid_r_bohr_p,
                                 radial_grid_w_p)

    r = radial_grid_r_bohr_np.tolist()
    w = radial_grid_w_np.tolist()

    return r, w


def get_angular_grid(num_angular_grid_points):

    angular_grid_x_bohr_np = np.zeros(num_angular_grid_points, dtype=np.float64)
    angular_grid_x_bohr_p = _ffi.cast("double *", angular_grid_x_bohr_np.ctypes.data)
    angular_grid_y_bohr_np = np.zeros(num_angular_grid_points, dtype=np.float64)
    angular_grid_y_bohr_p = _ffi.cast("double *", angular_grid_y_bohr_np.ctypes.data)
    angular_grid_z_bohr_np = np.zeros(num_angular_grid_points, dtype=np.float64)
    angular_grid_z_bohr_p = _ffi.cast("double *", angular_grid_z_bohr_np.ctypes.data)
    angular_grid_w_np = np.zeros(num_angular_grid_points, dtype=np.float64)
    angular_grid_w_p = _ffi.cast("double *", angular_grid_w_np.ctypes.data)

    _lib.numgrid_get_angular_grid(num_angular_grid_points,
                                  angular_grid_x_bohr_p,
                                  angular_grid_y_bohr_p,
                                  angular_grid_z_bohr_p,
                                  angular_grid_w_p)

    x = angular_grid_x_bohr_np.tolist()
    y = angular_grid_y_bohr_np.tolist()
    z = angular_grid_z_bohr_np.tolist()
    w = angular_grid_w_np.tolist()

    return x, y, z, w
