#.rst:
#
# Generates export header for your API using best practices.
#
# Variables used::
#
#   PROJECT_NAME (defined by project())

include(GenerateExportHeader)

generate_export_header(${PROJECT_NAME}
    BASE_NAME "${PROJECT_NAME}"
    EXPORT_MACRO_NAME "${PROJECT_NAME}_EXPORT"
    EXPORT_FILE_NAME "include/${PROJECT_NAME}_export.h"
    DEPRECATED_MACRO_NAME "${PROJECT_NAME}_DEPRECATED"
    NO_EXPORT_MACRO_NAME "${PROJECT_NAME}_NO_EXPORT"
    STATIC_DEFINE "${PROJECT_NAME}_STATIC_DEFINE"
    NO_DEPRECATED_MACRO_NAME "${PROJECT_NAME}_NO_DEPRECATED"
    DEFINE_NO_DEPRECATED
    )
