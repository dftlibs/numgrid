include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lebedev
    ${CMAKE_CURRENT_LIST_DIR}/../api
    )

add_library(
    numgrid
    SHARED
    Grid.cpp
    Grid.h
    becke_partitioning.cpp
    becke_partitioning.h
    grid_radial.cpp
    grid_radial.h
    lebedev
    parameters.h
    lebedev/sphere_lebedev_rule.cpp
    lebedev/sphere_lebedev_rule.h
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
