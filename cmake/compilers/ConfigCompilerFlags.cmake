include(SaveCompilerFlags)

if(CMAKE_C_COMPILER_WORKS)
    include(CFlags)
endif()

if(CMAKE_CXX_COMPILER_WORKS)
    include(CXXFlags)
endif()
