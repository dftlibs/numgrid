# copy reference file to ${PROJECT_BINARY_DIR}/test (aka ${CMAKE_CURRENT_BINARY_DIR})
file(COPY reference_grid.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(ExternalProject)

ExternalProject_Add(
    gtest
    PREFIX
        "${PROJECT_BINARY_DIR}/gtest"
    GIT_REPOSITORY
        https://github.com/google/googletest.git
    GIT_TAG
        release-1.8.0
    CMAKE_ARGS
        "-Dgtest_disable_pthreads=ON"
    INSTALL_COMMAND
        "" # prevent gtest from installing itself system-wide
    BUILD_BYPRODUCTS
        # this is here otherwise Ninja will complain
        # that there is no rule to build libgtest.a
        ${PROJECT_BINARY_DIR}/gtest/src/gtest-build/googlemock/gtest/libgtest.a
    )

add_executable(
    cpp_test
    main.cpp
    test.cpp
    )

target_include_directories(
    cpp_test
    PRIVATE
    ${PROJECT_SOURCE_DIR}/numgrid
    ${PROJECT_BINARY_DIR}/include
    ${PROJECT_BINARY_DIR}/gtest/src/gtest/googletest/include
    )

target_link_libraries(
    cpp_test
    PRIVATE
    numgrid
    ${PROJECT_BINARY_DIR}/gtest/src/gtest-build/googlemock/gtest/libgtest.a
    )

# make sure that gtest is built before we build cpp_test
add_dependencies(cpp_test gtest)

add_test(
  NAME cpp_test
  COMMAND $<TARGET_FILE:cpp_test>
  )

if(ENABLE_FC_SUPPORT)
    add_executable(
        fortran_test
        test.f90
        )

    add_dependencies(fortran_test gtest)

    target_link_libraries(
        fortran_test
        numgrid
        )

    add_test(
      NAME fortran_test
      COMMAND $<TARGET_FILE:fortran_test>
      )
endif()
