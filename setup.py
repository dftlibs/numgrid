#!/usr/bin/env python

import os
import sys

sys.path.append('cmake/lib')
from config import configure
from docopt import docopt


options = """
Usage:
  ./setup.py [options] [<builddir>]
  ./setup.py (-h | --help)

Options:
  --cc=<CC>    C compiler [default: gcc].
  --cxx=<CXX>  C++ compiler [default: g++].
  --coverage   Enable code coverage [default: False].
  --show       Show CMake command and exit.
  <builddir>   Build directory.
  -h --help    Show this screen.
"""


def gen_cmake_command(options, arguments):
    """
    Generate CMake command based on options and arguments.
    """
    command = []
    command.append('CC=%s' % arguments['--cc'])
    command.append('CXX=%s' % arguments['--cxx'])
    command.append('cmake')
    command.append('-DENABLE_CODE_COVERAGE=%s' % arguments['--coverage'])

    return ' '.join(command)


arguments = docopt(options, argv=None)

root_directory = os.path.dirname(os.path.realpath(__file__))
build_path = arguments['<builddir>']
cmake_command = '%s %s' % (gen_cmake_command(options, arguments), root_directory)
configure(root_directory, build_path, cmake_command, arguments['--show'])
