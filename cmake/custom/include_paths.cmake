include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/include)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)
