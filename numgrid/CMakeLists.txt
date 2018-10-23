add_library(
  numgrid
  SHARED
    AtomGrid.cpp
    becke_partitioning.cpp
    grid_radial.cpp
    version.cpp
    bragg.cpp
    lebedev/sphere_lebedev_rule.cpp
  )

add_dependencies(numgrid version)

target_include_directories(
  numgrid
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lebedev
    ${PROJECT_BINARY_DIR}/include
  PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
  )

if(ENABLE_FC_SUPPORT)
  add_library(
    numgrid_fortran_bindings
    SHARED
      numgrid.f90
    )

  target_link_libraries(
    numgrid_fortran_bindings
    PRIVATE
      numgrid
    )
endif()

# generate numgrid_export.h
include(GenerateExportHeader)
generate_export_header(
  numgrid
  BASE_NAME numgrid
  )

file(
  GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/interface_file_names.cfg
  INPUT ${CMAKE_CURRENT_SOURCE_DIR}/interface_file_names.cfg.in
  )

set_target_properties(
  numgrid
  PROPERTIES
    PUBLIC_HEADER "numgrid.h;${CMAKE_CURRENT_BINARY_DIR}/numgrid_export.h"
    RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/interface_file_names.cfg"
  )

install(
  TARGETS
    numgrid
  LIBRARY
    DESTINATION numgrid/lib
  RUNTIME
    DESTINATION numgrid/lib
  PUBLIC_HEADER
    DESTINATION numgrid/include
  RESOURCE
    DESTINATION numgrid
  )
