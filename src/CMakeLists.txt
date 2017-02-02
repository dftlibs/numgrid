include_directories(
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/src/lebedev
    ${PROJECT_SOURCE_DIR}/api
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
    parameters.h
    version.cpp
    )

add_dependencies(numgrid version)

target_link_libraries(
    numgrid
    lebedev
    )

add_subdirectory(lebedev)

if(ENABLE_FC_SUPPORT)
    add_library(
        numgrid_fortran
        ${CMAKE_CURRENT_LIST_DIR}/../api/numgrid.f90
        )

    target_link_libraries(
        numgrid_fortran
        numgrid
        )
endif()

install(TARGETS numgrid LIBRARY DESTINATION lib)
install(FILES ${PROJECT_BINARY_DIR}/include/numgrid.h DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/include/numgrid_export.h DESTINATION include)
