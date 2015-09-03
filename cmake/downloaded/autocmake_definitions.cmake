#.rst:
#
# Add preprocessor definitions (example: --add-definitions="-DTHIS -DTHAT=137").
# These are passed all the way down to the compiler.
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
