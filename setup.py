#!/usr/bin/env python

from distutils.core import setup
import distutils.spawn as _spawn
import distutils.command.build as _build
import distutils.dir_util as _dir_util
import setuptools.command.install as _install
import os
import sys
from distutils.sysconfig import get_python_lib
from shutil import copy2


def run_cmake():
    """
    Runs CMake to determine configuration for this build.
    """
    if _spawn.find_executable('cmake') is None:
        print("CMake is required to build this package.")
        print("Please install/load CMake and re-run setup.")
        sys.exit(-1)

    _build_dir = os.path.join(os.path.split(__file__)[0], 'build_setup_py')
    _dir_util.mkpath(_build_dir)
    os.chdir(_build_dir)

    try:
        _spawn.spawn(['cmake', '..'])
    except _spawn.DistutilsExecError:
        print("Error while running CMake")
        sys.exit(-1)


class build(_build.build):

    def run(self):
        cwd = os.getcwd()
        run_cmake()

        try:
            _spawn.spawn(['make'])
            os.chdir(cwd)
        except _spawn.DistutilsExecError:
            print("Error while running Make")
            sys.exit(-1)

        _build.build.run(self)


class install(_install.install):
    def run(self):
        cwd = os.getcwd()
        _install.install.run(self)
        _target_path = os.path.join(get_python_lib(), 'numgrid')

        if not os.path.exists(_target_path):
             os.makedirs(_target_path)

        if sys.platform == "darwin":
            suffix = 'dylib'
        else:
            suffix = 'so'

        for f in [os.path.join('build_setup_py', 'lib', 'libnumgrid.{0}'.format(suffix)),
                  os.path.join('build_setup_py', 'include', 'numgrid_export.h'),
                  os.path.join('numgrid', 'numgrid.h')]:
            copy2(os.path.join(cwd, f), _target_path)


setup(name='numgrid',
      version='1.0.0-alpha',
      description='Numerical integration grid for molecules.',
      author='Radovan Bast',
      author_email='radovan.bast@uit.no',
      url='https://github.com/dftlibs/numgrid',
      packages=['numgrid'],
      license='MPL-v2.0',
      install_requires=['cffi', 'numpy'],
      cmdclass={'install': install, 'build': build})
