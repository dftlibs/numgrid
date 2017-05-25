# List of headers
list(APPEND headers_list sphere_lebedev_rule.h)

# List of sources
list(APPEND sources_list sphere_lebedev_rule.cpp)

add_library(_lebedev OBJECT ${sources_list} ${headers_list})
set_target_properties(_lebedev PROPERTIES POSITION_INDEPENDENT_CODE 1)
# Sets install directory for all the headers in the list
foreach(_header ${headers_list})
   install(FILES ${_header} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/lebedev)
endforeach()
