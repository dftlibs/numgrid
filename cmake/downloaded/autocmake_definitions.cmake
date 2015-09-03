#.rst:
#
# Add preprocessor definitions.
#
# Variables used::
#
#   PREPROCESSOR_DEFINITIONS
#
# autocmake.cfg configuration::
#
#   docopt: --add-definitions=<STRING> Add preprocesor definitions [default: ''].
#   define: '-DPREPROCESSOR_DEFINITIONS="%s"' % arguments['--add-definitions']

if(NOT "${PREPROCESSOR_DEFINITIONS}" STREQUAL "")
    add_definitions(${PREPROCESSOR_DEFINITIONS})
endif()
