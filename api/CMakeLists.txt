install(FILES ${PROJECT_SOURCE_DIR}/api/numgrid.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

if(ENABLE_FC_SUPPORT)
  add_library(numgrid_fortran OBJECT ${PROJECT_SOURCE_DIR}/api/numgrid.f90)
  set_target_properties(numgrid_fortran PROPERTIES INCLUDE_DIRECTORIES ""
    POSITION_INDEPENDENT_CODE 1
    )
  message(STATUS "${BoldBlue}numgrid Fortran bindings ENABLED${ColourReset}")
endif()
