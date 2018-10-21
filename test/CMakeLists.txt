# copy reference file to ${PROJECT_BINARY_DIR}/test (aka ${CMAKE_CURRENT_BINARY_DIR})
file(COPY reference_grid.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.8.0
)

FetchContent_GetProperties(googletest)

if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)

  # adds the targers: gtest, gtest_main, gmock, gmock_main
  add_subdirectory(
    ${googletest_SOURCE_DIR}
    ${googletest_BINARY_DIR}
    )
endif()

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
    gtest_main
  )

add_test(
  NAME cpp_test
  COMMAND $<TARGET_FILE:cpp_test>
  )

if(ENABLE_FC_SUPPORT)
  add_executable(
    fortran_test
    test.f90
    )

  target_link_libraries(
    fortran_test
    numgrid
    )

  add_test(
    NAME fortran_test
    COMMAND $<TARGET_FILE:fortran_test>
    )
endif()
