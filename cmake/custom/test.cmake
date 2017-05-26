if(ENABLE_UNIT_TESTS)
  include(CTest)
  enable_testing()
  find_package(Threads REQUIRED)

  add_subdirectory(test)
endif()
