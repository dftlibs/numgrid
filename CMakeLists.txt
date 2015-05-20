cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(numgrid)

enable_language(C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

option(ENABLE_COVERAGE "Enable test coverage" OFF)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    endif()
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/api
    ${PROJECT_SOURCE_DIR}/external/lebedev
    )

add_library(
    numgrid
    src/becke_partitioning.cpp
    src/grid_radial.cpp
    src/Grid.cpp
    src/MemAllocator.cpp
    external/lebedev/sphere_lebedev_rule.cpp
    )

add_custom_target(
    git_update
    COMMAND git submodule init
    COMMAND git submodule update
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

include_directories(
    ${PROJECT_SOURCE_DIR}/external/googletest
    ${PROJECT_SOURCE_DIR}/external/googletest/include
    )

set(GTEST_SOURCES
    ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest_main.cc
    )

add_library(gtest ${GTEST_SOURCES})

foreach(_source ${GTEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_dependencies(gtest git_update)

add_executable(
    unit_tests
    test/main.cpp
    test/test.cpp
    )

add_dependencies(unit_tests gtest)

target_link_libraries(
    unit_tests
    gtest
    numgrid
    pthread
    )

include(CTest)
enable_testing()

add_test(unit ${PROJECT_BINARY_DIR}/bin/unit_tests)
