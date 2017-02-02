import sys
import os
from subprocess import Popen, PIPE
from cffi import FFI


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


# outward facing API
new_context = _lib.numgrid_new_context
free_context = _lib.numgrid_free_context
generate_grid = _lib.numgrid_generate_grid
get_num_points = _lib.numgrid_get_num_points
get_grid = _lib.numgrid_get_grid


def get_version():
    ffi = FFI()
    return ffi.string(_lib.numgrid_get_version()).decode('utf-8')
