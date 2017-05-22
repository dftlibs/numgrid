include(CMakePackageConfigHelpers)


# <<<  Export config  >>>

    # explicit "share" not "DATADIR" for CMake search path
set(CMAKECONFIG_INSTALL_DIR "share/cmake/${PROJECT_NAME}")
configure_package_config_file(${numgrid_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
                                    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                                    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR})
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                                      VERSION ${PROGRAM_VERSION}
                                      COMPATIBILITY AnyNewerVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
               DESTINATION ${CMAKECONFIG_INSTALL_DIR})

