# copy reference grid data to ${PROJECT_BINARY_DIR}/test (aka ${CMAKE_CURRENT_BINARY_DIR})
file(COPY reference_grid DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(fetch_git_repo.cmake)

fetch_git_repo(
  googletest
  ${CMAKE_BINARY_DIR}/_deps
  https://github.com/google/googletest.git
  release-1.8.1
)

# adds the targers: gtest, gtest_main, gmock, gmock_main
add_subdirectory(
  ${googletest_SOURCE_DIR}
  ${googletest_BINARY_DIR}
  )

add_executable(
  cpp_test
    main.cpp
    test_explicit.cpp
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
    numgrid-objects
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
    numgrid_fortran_bindings
    numgrid-objects
    )

  add_test(
    NAME fortran_test
    COMMAND $<TARGET_FILE:fortran_test>
    )
endif()
