# (c) https://github.com/dev-cafe/autocmake/blob/master/AUTHORS.md
# licensed under BSD-3: https://github.com/dev-cafe/autocmake/blob/master/LICENSE

#.rst:
#
# Enables code coverage by appending corresponding compiler flags.
#
# Variables modified (provided the corresponding language is enabled)::
#
#   CMAKE_Fortran_FLAGS
#   CMAKE_C_FLAGS
#   CMAKE_CXX_FLAGS
#
# autocmake.yml configuration::
#
#   docopt: "--coverage Enable code coverage [default: OFF]."
#   define: "'-DENABLE_CODE_COVERAGE={0}'.format(arguments['--coverage'])"

option(ENABLE_CODE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_CODE_COVERAGE)
  if(NOT CMAKE_BUILD_TYPE STREQUAL "debug")
    message(WARNING "Code coverage analysis results with an optimized (non-Debug) build may be misleading")
  endif()

  find_program(GCOV_PATH gcov)
  if(NOT GCOV_PATH)
    message(FATAL_ERROR "Code coverage analysis requires gcov!")
  endif()

  if(DEFINED CMAKE_Fortran_COMPILER_ID)
    if(CMAKE_Fortran_COMPILER_ID MATCHES GNU)
      set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fprofile-arcs -ftest-coverage")
    else()
      message(FATAL_ERROR "Code coverage analysis requires the GNU Fortran compiler!")
    endif()
  endif()

  if(DEFINED CMAKE_C_COMPILER_ID)
    if(CMAKE_C_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
      if(CMAKE_C_COMPILER_VERSION VERSION_LESS 3)
        message(FATAL_ERROR "Code coverage analysis on Mac OS X requires Clang version 3.0.0 or greater!")
      else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
      endif()
    elseif(CMAKE_C_COMPILER_ID MATCHES GNU)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    else()
      message(FATAL_ERROR "Code coverage analysis requires the GNU C compiler!")
    endif()
  endif()

  if(DEFINED CMAKE_CXX_COMPILER_ID)
    if(CMAKE_CXX_COMPILER_ID MATCHES "(Apple)?[Cc]lang")
      if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3)
        message(FATAL_ERROR "Code coverage analysis on Mac OS X requires Clang version 3.0.0 or greater!")
      else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
      endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    else()
      message(FATAL_ERROR "Code coverage analysis requires the GNU C++ compiler!")
    endif()
  endif()
endif()
