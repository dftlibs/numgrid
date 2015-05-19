cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(numgrid)

enable_language(C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/compilers
    )

option(ENABLE_MPI "Enable MPI" OFF)
if(ENABLE_MPI)
    find_package(MPI)
    if(MPI_FOUND)
        include_directories(${MPI_INCLUDE_PATH})
        add_definitions(-DENABLE_MPI)
    else()
        message(FATAL_ERROR "-- You asked for MPI, but CMake could not find any MPI installation, check $PATH")
    endif()
endif()

option(ENABLE_OMP "Enable OpenMP" OFF)
if(ENABLE_OMP)
    find_package(OpenMP)
    if(OPENMP_FOUND)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        add_definitions(-DENABLE_OMP)
    endif()
endif()

include(OptimizeForArchitecture)
OptimizeForArchitecture()
if(AVX_FOUND)
    add_definitions(-DCPU_MEM_ALIGN=32)
    message(STATUS "Setting CPU_MEM_ALIGN=32")
elseif(SSE3_FOUND)
    add_definitions(-DCPU_MEM_ALIGN=16)
    message(STATUS "Setting CPU_MEM_ALIGN=16")
else()
    message(FATAL_ERROR "neither SSE3 nor AVX detected; adapt CMakeLists.txt")
endif()

include(ConfigSafeGuards)
include(ConfigCompilerFlags)

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
    src/io.cpp
    src/rolex.cpp
    src/MemAllocator.cpp
    api/numgrid_c_api.cpp
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
