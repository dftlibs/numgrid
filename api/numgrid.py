"""
Python handle to numgrid library.
"""

import os
import sys
import subprocess
from cffi import FFI

BUILD_DIR = os.path.abspath(os.path.dirname(__file__))

ffi = FFI()

ffi.cdef(
    subprocess.Popen(
        [
            'cc',
            '-E',
            '-DNUMGRID_API=',
            '-DNUMGRID_NOINCLUDE',
            os.path.join(BUILD_DIR, 'include', 'numgrid.h')
        ],
        stdout=subprocess.PIPE).communicate()[0].decode('utf-8'))

if sys.platform == "darwin":
    suffix = 'dylib'
else:
    suffix = 'so'

_lib = ffi.dlopen(os.path.join(BUILD_DIR, 'lib', 'libnumgrid.%s' % suffix))

# outward facing API
new_context = _lib.new_context
free_context = _lib.free_context
generate_grid = _lib.generate_grid
get_num_points = _lib.get_num_points
get_grid = _lib.get_grid

def get_version():
    return ffi.string(_lib.get_version()).decode('utf-8')
