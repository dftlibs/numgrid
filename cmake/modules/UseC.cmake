enable_language(C)

if(NOT DEFINED CMAKE_C_COMPILER_ID)
    message(FATAL_ERROR "CMAKE_C_COMPILER_ID variable is not defined!")
endif()

if(NOT CMAKE_C_COMPILER_WORKS)
    message(FATAL_ERROR "CMAKE_C_COMPILER_WORKS is false!")
endif()

if(DEFINED EXTRA_C_FLAGS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS}")
endif()
