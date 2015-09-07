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
        stdout=subprocess.PIPE).communicate()[0])

if sys.platform == "darwin":
    suffix = 'dylib'
else:
    suffix = 'so'

lib = ffi.dlopen(os.path.join(BUILD_DIR, 'lib', 'libnumgrid.%s' % suffix))
