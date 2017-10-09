add_library(_numgrid OBJECT
  AtomGrid.cpp
  becke_partitioning.cpp
  grid_radial.cpp
  version.cpp
  bragg.cpp
  )
target_include_directories(_numgrid SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/external/upstream)
target_include_directories(_numgrid SYSTEM PRIVATE ${PROJECT_BINARY_DIR}/include)
add_dependencies(_numgrid version)
set_target_properties(_numgrid PROPERTIES POSITION_INDEPENDENT_CODE 1)
list(APPEND _objects
    $<TARGET_OBJECTS:_numgrid>
    $<TARGET_OBJECTS:_lebedev>
  )
if(ENABLE_FC_SUPPORT)
  list(APPEND _objects
    $<TARGET_OBJECTS:numgrid_fortran>
    )
endif()

if(NOT STATIC_LIBRARY_ONLY)
  add_library(${PROJECT_NAME}-shared
    SHARED
    ${_objects}
    )
  set_target_properties(${PROJECT_NAME}-shared PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR}
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
    OUTPUT_NAME "numgrid"
    EXPORT_NAME "numgrid"
    )
  install(TARGETS ${PROJECT_NAME}-shared
    EXPORT "${PROJECT_NAME}Targets-shared"
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

if(NOT SHARED_LIBRARY_ONLY)
  add_library(${PROJECT_NAME}-static
    STATIC
    ${_objects}
    )
  set_target_properties(${PROJECT_NAME}-static PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME}_STATIC_DEFINE
    OUTPUT_NAME "numgrid"
    EXPORT_NAME "numgrid"
    )
  install(TARGETS ${PROJECT_NAME}-static
    EXPORT "${PROJECT_NAME}Targets-static"
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

if(STATIC_LIBRARY_ONLY)
  add_library(${PROJECT_NAME} ALIAS ${PROJECT_NAME}-static)
else()
  add_library(${PROJECT_NAME} ALIAS ${PROJECT_NAME}-shared)
endif()

install(FILES ${PROJECT_BINARY_DIR}/include/${PROJECT_NAME}_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# <<<  Export interface  >>>

if(NOT STATIC_LIBRARY_ONLY)
  if (APPLE)
    set_target_properties(${PROJECT_NAME}-shared PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
  endif()
  target_compile_definitions(${PROJECT_NAME}-shared INTERFACE USING_${PROJECT_NAME})
  target_include_directories(${PROJECT_NAME}-shared INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endif()

if(NOT SHARED_LIBRARY_ONLY)
  target_compile_definitions(${PROJECT_NAME}-static INTERFACE USING_${PROJECT_NAME})
  target_include_directories(${PROJECT_NAME}-static INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endif()

# <<<  Export Config  >>>

# explicit "share" not "DATADIR" for CMake search path
set (CMAKECONFIG_INSTALL_DIR "share/cmake/${PROJECT_NAME}")
if(NOT STATIC_LIBRARY_ONLY)
  install (EXPORT "${PROJECT_NAME}Targets-shared"
    NAMESPACE "${PROJECT_NAME}::"
    DESTINATION ${CMAKECONFIG_INSTALL_DIR})
endif()

if(NOT SHARED_LIBRARY_ONLY)
  install (EXPORT "${PROJECT_NAME}Targets-static"
    NAMESPACE "${PROJECT_NAME}::"
    DESTINATION ${CMAKECONFIG_INSTALL_DIR})
endif()

install(FILES ${PROJECT_SOURCE_DIR}/numgrid/numgrid.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

message(STATUS "numgrid Fortran bindings ${BoldBlue}${ENABLE_FC_SUPPORT}${ColourReset}")
if(ENABLE_FC_SUPPORT)
  add_library(numgrid_fortran OBJECT ${PROJECT_SOURCE_DIR}/numgrid/numgrid.f90)
  set_target_properties(numgrid_fortran PROPERTIES INCLUDE_DIRECTORIES ""
    POSITION_INDEPENDENT_CODE 1
    )
endif()
