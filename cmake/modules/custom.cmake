set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include)
file(COPY ${PROJECT_SOURCE_DIR}/api/numgrid.h  DESTINATION ${PROJECT_BINARY_DIR}/include)
file(COPY ${PROJECT_SOURCE_DIR}/api/numgrid.py DESTINATION ${PROJECT_BINARY_DIR})

set(GTEST_ROOT ${PROJECT_SOURCE_DIR}/external/googletest)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/api
    ${PROJECT_SOURCE_DIR}/external/lebedev
    )

add_library(
    numgrid
    SHARED
    src/becke_partitioning.cpp
    src/grid_radial.cpp
    src/Grid.cpp
    src/MemAllocator.cpp
    external/lebedev/sphere_lebedev_rule.cpp
    )
