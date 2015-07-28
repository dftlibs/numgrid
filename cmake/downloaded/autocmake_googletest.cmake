#.rst:
#
# Includes Google Test sources and adds a library "googletest".
#
# Variables used::
#
#   GOOGLETEST_ROOT
#
# autocmake.cfg configuration::
#
#   define: '-DGOOGLETEST_ROOT=external/googletest'

set(GOOGLETEST_ROOT external/googletest CACHE STRING "Google Test source root")

message(STATUS "GOOGLETEST_ROOT set to ${GOOGLETEST_ROOT}")

include_directories(
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
    )

set(GOOGLETEST_SOURCES
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
    )

foreach(_source ${GOOGLETEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(googletest ${GOOGLETEST_SOURCES})
