if(ENABLE_UNIT_TESTS)
  include(ExternalProject)

  ExternalProject_Add(
    gtest
    PREFIX ${PROJECT_BINARY_DIR}/external/upstream/gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
    INSTALL_COMMAND ""
    )
endif()
