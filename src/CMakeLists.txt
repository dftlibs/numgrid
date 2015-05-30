include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lebedev
    ${CMAKE_CURRENT_LIST_DIR}/../api
    )

add_library(
    numgrid
    SHARED
    becke_partitioning.cpp
    grid_radial.cpp
    Grid.cpp
    MemAllocator.cpp
    lebedev/sphere_lebedev_rule.cpp
    )
