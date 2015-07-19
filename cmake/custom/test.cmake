include(CTest)
enable_testing()

add_executable(
    cpp_test
    test/main.cpp
    test/test.cpp
    )

target_link_libraries(
    cpp_test
    googletest
    numgrid
    pthread
    )

add_test(cpp_test ${PROJECT_BINARY_DIR}/bin/cpp_test)

if(ENABLE_FC_SUPPORT)
    add_executable(
        fortran_test
        test/test.F90
        )

    target_link_libraries(
        fortran_test
        numgrid_fortran
        )

    add_test(fortran_test ${PROJECT_BINARY_DIR}/bin/fortran_test)
endif()
