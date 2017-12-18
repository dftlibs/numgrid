# (c) https://github.com/coderefinery/autocmake/blob/master/AUTHORS.md
# licensed under BSD-3: https://github.com/coderefinery/autocmake/blob/master/LICENSE

#.rst:
#
# Generates export header for your API using best practices.
# This module wraps https://cmake.org/cmake/help/v3.0/module/GenerateExportHeader.html and
# it is needed because of these, Very Good Indeed, reasons: https://gcc.gnu.org/wiki/Visibility.
# If you are not afraid of jargon, please also have a look at https://www.akkadia.org/drepper/dsohowto.pdf.
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
