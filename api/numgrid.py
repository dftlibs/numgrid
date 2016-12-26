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
numgrid_new = _lib.numgrid_new
numgrid_free = _lib.numgrid_free
numgrid_generate = _lib.numgrid_generate
numgrid_get_num_points = _lib.numgrid_get_num_points
numgrid_get_grid = _lib.numgrid_get_grid

def numgrid_get_version():
    return ffi.string(_lib.numgrid_get_version()).decode('utf-8')
