function(guard_against_in_source in_source_dir in_binary_dir)
    if(${in_source_dir} STREQUAL ${in_binary_dir})
        message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
    endif()
endfunction()

function(guard_against_bad_build_types in_build_type)
    string(TOLOWER "${in_build_type}" cmake_build_type_tolower)
    string(TOUPPER "${in_build_type}" cmake_build_type_toupper)

    if(    NOT cmake_build_type_tolower STREQUAL "debug"
       AND NOT cmake_build_type_tolower STREQUAL "release"
       AND NOT cmake_build_type_tolower STREQUAL "relwithdebinfo")
        message(FATAL_ERROR "Unknown build type \"${in_build_type}\". Allowed values are Debug, Release, RelWithDebInfo (case-insensitive).")
    endif()
endfunction()

guard_against_in_source(${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})
guard_against_bad_build_types(${CMAKE_BUILD_TYPE})
