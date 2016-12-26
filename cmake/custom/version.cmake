add_custom_target(
    version
    ALL
    ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/cmake/custom/gen_version.py
                         ${PROJECT_SOURCE_DIR}/VERSION
                         ${PROJECT_BINARY_DIR}/version.h
    DEPENDS
    ${PROJECT_SOURCE_DIR}/VERSION
    )

set_source_files_properties(${PROJECT_BINARY_DIR}/version.h PROPERTIES GENERATED 1)
