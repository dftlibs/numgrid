add_library(
  numgrid-objects
  OBJECT
    AtomGrid.cpp
    becke_partitioning.cpp
    grid_radial.cpp
    version.cpp
    bragg.cpp
    lebedev/sphere_lebedev_rule.cpp
  )

target_compile_options(
  numgrid-objects
  PRIVATE
    "-fPIC"
  )

target_include_directories(
  numgrid-objects
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/lebedev
    ${PROJECT_BINARY_DIR}/include
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
  )

add_dependencies(numgrid-objects version)

add_library(
  numgrid-shared
  SHARED
    $<TARGET_OBJECTS:numgrid-objects>
  )

# this is an ugly hack to make the unit tests link
# a nicer solution would be not to duplicate this info
# since the target dependencies are already expressed
# in numgrid-objects
target_include_directories(
  numgrid-shared
  PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
  )

set_target_properties(
  numgrid-shared
  PROPERTIES
    OUTPUT_NAME "numgrid"
  )

add_library(
  numgrid-static
  STATIC
    $<TARGET_OBJECTS:numgrid-objects>
  )

set_target_properties(
  numgrid-static
  PROPERTIES
    OUTPUT_NAME "numgrid"
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
      numgrid-shared
    )
endif()

# generate numgrid_export.h
include(GenerateExportHeader)
generate_export_header(
  numgrid-shared
  BASE_NAME numgrid
  )

file(
  GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/interface_file_names.cfg
  INPUT ${CMAKE_CURRENT_SOURCE_DIR}/interface_file_names.cfg.in
  )

set_target_properties(
  numgrid-shared
  PROPERTIES
    PUBLIC_HEADER "numgrid.h;${CMAKE_CURRENT_BINARY_DIR}/numgrid_export.h"
    RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/interface_file_names.cfg"
  )

install(
  TARGETS
    numgrid-shared
    numgrid-static
  LIBRARY
    DESTINATION lib
  RUNTIME
    DESTINATION lib
  ARCHIVE
    DESTINATION lib
  PUBLIC_HEADER
    DESTINATION include
  RESOURCE
    DESTINATION .
  )
