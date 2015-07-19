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

if(ENABLE_FC_SUPPORT)
    add_library(
        numgrid_fortran
        ${CMAKE_CURRENT_LIST_DIR}/../api/numgrid.F90
        )

    target_link_libraries(
        numgrid_fortran
        numgrid
        )
endif()

install(TARGETS numgrid LIBRARY DESTINATION lib)
