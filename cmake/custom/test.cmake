add_executable(
    unit_tests
    test/main.cpp
    test/test.cpp
    )

target_link_libraries(
    unit_tests
    googletest
    numgrid
    pthread
    )

include(CTest)
enable_testing()

add_test(unit ${PROJECT_BINARY_DIR}/bin/unit_tests)
