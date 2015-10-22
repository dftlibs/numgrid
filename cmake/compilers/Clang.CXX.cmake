if(NOT DEFINED ENV{CXXFLAGS})
    if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -D__extern_always_inline=inline")
        # -D__extern_always_inline=inline added for Travis
        set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    endif()
endif()
