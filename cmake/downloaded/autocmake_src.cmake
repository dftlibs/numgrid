#.rst:
#
# Adds ${PROJECT_SOURCE_DIR}/src as subdirectory containing CMakeLists.txt.
#
# Example autocmake.cfg entry::
#
#   [src]
#   source: https://github.com/scisoft/autocmake/raw/master/modules/src.cmake

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
