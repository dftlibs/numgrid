set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include)
file(COPY ${PROJECT_SOURCE_DIR}/api/numgrid.h  DESTINATION ${PROJECT_BINARY_DIR}/include)
file(COPY ${PROJECT_SOURCE_DIR}/api/numgrid.py DESTINATION ${PROJECT_BINARY_DIR})

option(ENABLE_COVERAGE "Enable test coverage" OFF)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    endif()
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/api
    ${PROJECT_SOURCE_DIR}/external/lebedev
    )

add_library(
    numgrid
    SHARED
    src/becke_partitioning.cpp
    src/grid_radial.cpp
    src/Grid.cpp
    src/MemAllocator.cpp
    external/lebedev/sphere_lebedev_rule.cpp
    )

include_directories(
    ${PROJECT_SOURCE_DIR}/external/googletest
    ${PROJECT_SOURCE_DIR}/external/googletest/include
    )

set(GTEST_SOURCES
    ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest_main.cc
    )

add_library(gtest ${GTEST_SOURCES})

foreach(_source ${GTEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_executable(
    unit_tests
    test/main.cpp
    test/test.cpp
    )

add_dependencies(unit_tests gtest)

target_link_libraries(
    unit_tests
    gtest
    numgrid
    pthread
    )

include(CTest)
enable_testing()

add_test(unit ${PROJECT_BINARY_DIR}/bin/unit_tests)
