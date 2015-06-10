# set minimum cmake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)


project(numgrid)


# do not rebuild if rules (compiler flags) change
set(CMAKE_SKIP_RULE_DEPENDENCY TRUE)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()


# python interpreter is required at many
# places during configuration and build
find_package(PythonInterp REQUIRED)


# directory which holds enabled cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake/modules)


# included cmake modules
include(autocmake_cc)
include(autocmake_cxx)
include(autocmake_GNU.CXX)
include(autocmake_Intel.CXX)
include(autocmake_code_coverage)
include(autocmake_safe_guards)
include(autocmake_cpp)
include(autocmake_default_build_paths)
include(autocmake_src)
include(autocmake_api)
include(autocmake_gtest)
include(autocmake_test)
