#.rst:
#
# Sets binary and library output directories to ${PROJECT_BINARY_DIR}/bin
# and ${PROJECT_BINARY_DIR}/lib, respectively.
#
# Variables modified::
#
#   CMAKE_RUNTIME_OUTPUT_DIRECTORY
#   CMAKE_LIBRARY_OUTPUT_DIRECTORY

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
