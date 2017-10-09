set(gtest_ROOT ${PROJECT_BINARY_DIR}/external/upstream/gtest)
set(gtest_SOURCE_DIR ${gtest_ROOT}/src/gtest/googletest)
set(gtest_BINARY_DIR ${gtest_ROOT}/src/gtest-build/googlemock/gtest)

include_directories(${gtest_SOURCE_DIR}/include)
link_directories(${gtest_BINARY_DIR})

# Copy reference file to ${PROJECT_BINARY_DIR}/test (aka ${CMAKE_CURRENT_BINARY_DIR})
file(COPY reference_grid.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(
  cpp_test
  main.cpp
  test.cpp
  )

target_include_directories(cpp_test SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/numgrid ${PROJECT_BINARY_DIR}/include)

add_dependencies(cpp_test gtest)

target_link_libraries(
  cpp_test
  numgrid
  libgtest.a
  Threads::Threads
  )

add_test(cpp_test ${PROJECT_BINARY_DIR}/bin/cpp_test)

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

  add_test(fortran_test ${PROJECT_BINARY_DIR}/bin/fortran_test)
endif()
