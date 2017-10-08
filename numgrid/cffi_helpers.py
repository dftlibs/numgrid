import sys
import os
from subprocess import Popen, PIPE
from cffi import FFI


def _get_library_suffix():
    if sys.platform == "darwin":
        return 'dylib'
    else:
        return 'so'


def get_lib_handle(definitions, header, library, build_dir, include_dir):
    ffi = FFI()
    interface = Popen(['cc', '-E'] + definitions + [os.path.join(include_dir, header)],
                      stdout=PIPE).communicate()[0].decode("utf-8")
    ffi.cdef(interface)

    suffix = _get_library_suffix()
    lib = ffi.dlopen(os.path.join(build_dir, 'lib{0}.{1}'.format(library, suffix)))
    return lib
