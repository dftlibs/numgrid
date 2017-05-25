file(READ "${PROJECT_SOURCE_DIR}/VERSION" PROGRAM_VERSION)
string(STRIP "${PROGRAM_VERSION}" PROGRAM_VERSION)
string(REPLACE "." ";" _version_list ${PROGRAM_VERSION})
list(GET _version_list 0 PROJECT_VERSION_MAJOR)
list(GET _version_list 1 PROJECT_VERSION_MINOR)
list(GET _version_list 2 PROJECT_VERSION_PATCH)

set(${PROJECT_NAME}_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
message(STATUS "${BoldGreen}numgrid v${${PROJECT_NAME}_VERSION}${ColourReset}")

add_custom_target(
    version
    ALL
    ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/cmake/custom/gen_version.py
                         ${PROJECT_SOURCE_DIR}/VERSION
                         ${PROJECT_BINARY_DIR}/include/version.h
    DEPENDS
    ${PROJECT_SOURCE_DIR}/VERSION
    )

set_source_files_properties(${PROJECT_BINARY_DIR}/include/version.h PROPERTIES GENERATED 1)
