enable_language(CXX)

if(NOT DEFINED CMAKE_CXX_COMPILER_ID)
    message(FATAL_ERROR "CMAKE_CXX_COMPILER_ID variable is not defined!")
endif()

if(NOT CMAKE_CXX_COMPILER_WORKS)
    message(FATAL_ERROR "CMAKE_CXX_COMPILER_WORKS is false!")
endif()

if(DEFINED EXTRA_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
endif()
