# set minimum cmake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)


project(numgrid)


# do not rebuild if rules (compiler flags) change
set(CMAKE_SKIP_RULE_DEPENDENCY TRUE)


# directory which holds enabled cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake/modules)


# guards against in-source builds and bad build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()


# python interpreter is required at many
# places during configuration and build
find_package(PythonInterp REQUIRED)


# determine program version from file, example: "14.1"
# the reason why this information is stored
# in a file and not as cmake variable
# is that cmake-unaware programs can
# parse and use it (e.g. Sphinx)
file(READ "${PROJECT_SOURCE_DIR}/VERSION" PROGRAM_VERSION)
string(STRIP "${PROGRAM_VERSION}" PROGRAM_VERSION)


# generated cmake files will be written to this path
# only build info is generated
# everything else is static for the user
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/generated_by_cmake)
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${PROJECT_BINARY_DIR}/generated_by_cmake
    )


# included cmake modules
include(UseC)
include(UseCXX)
include(GNU.CXX)
include(Intel.CXX)
include(UseCodeCoverage)
include(custom)
