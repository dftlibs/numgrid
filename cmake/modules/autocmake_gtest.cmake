set(GTEST_ROOT ${PROJECT_SOURCE_DIR}/external/googletest)

include_directories(
    ${GTEST_ROOT}
    ${GTEST_ROOT}/include
    )

set(GTEST_SOURCES
    ${GTEST_ROOT}/src/gtest-all.cc
    ${GTEST_ROOT}/src/gtest_main.cc
    )

foreach(_source ${GTEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(gtest ${GTEST_SOURCES})
