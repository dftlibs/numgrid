option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Unit tests ${BoldBlue}${ENABLE_UNIT_TESTS}${ColourReset}")

if(ENABLE_UNIT_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
