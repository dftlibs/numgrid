# (c) https://github.com/coderefinery/autocmake/blob/master/AUTHORS.md
# licensed under BSD-3: https://github.com/coderefinery/autocmake/blob/master/LICENSE

#.rst:
#
# Adds ccache support.
# The user should export the appropriate environment variables to
# tweak the program's behaviour, as described in its manpage.
# Notice that some additional compiler flags might be needed in order
# to avoid unnecessary warnings.
#
# Variables defined::
#
#   CCACHE_FOUND
#
# autocmake.yml configuration::
#
#   docopt: "--ccache=<USE_CCACHE> Toggle use of ccache <ON/OFF> [default: ON]."
#   define: "'-DUSE_CCACHE={0}'.format(arguments['--ccache'])"

if(USE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
        message(STATUS "Compiling with ccache")
    else()
        message(STATUS "ccache not available")
    endif()
endif()
