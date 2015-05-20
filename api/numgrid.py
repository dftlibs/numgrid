"""
Python handle to numgrid library.
"""

import os
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

lib = ffi.dlopen(os.path.join(BUILD_DIR, 'lib', 'libnumgrid.so'))
