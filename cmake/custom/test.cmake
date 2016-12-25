option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

if(ENABLE_UNIT_TESTS)
    include(CTest)
    enable_testing()

    include(ExternalProject)

    ExternalProject_Add(
        gtest
        PREFIX "${PROJECT_BINARY_DIR}/gtest"
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master
        INSTALL_COMMAND true  # currently no install command
        )

    include_directories(${PROJECT_BINARY_DIR}/gtest/src/gtest/googletest/include)
    include_directories(${PROJECT_SOURCE_DIR}/src)

    link_directories(${PROJECT_BINARY_DIR}/gtest/src/gtest-build/googlemock/gtest/)

    add_executable(
        cpp_test
        test/main.cpp
        test/test.cpp
        )

    add_dependencies(cpp_test gtest)

    target_link_libraries(
        cpp_test
        libgtest.a
        numgrid
        pthread
        )

    add_test(cpp_test ${PROJECT_BINARY_DIR}/bin/cpp_test)

    if(ENABLE_FC_SUPPORT)
        add_executable(
            fortran_test
            test/test.F90
            )

        add_dependencies(fortran_test gtest)

        target_link_libraries(
            fortran_test
            numgrid_fortran
            )

        add_test(fortran_test ${PROJECT_BINARY_DIR}/bin/fortran_test)
    endif()
endif()
