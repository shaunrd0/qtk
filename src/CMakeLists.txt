################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2023 Shaun Reed, all rights reserved                       ##
################################################################################

# Qtk Library

# We always build libqtk since the plugins and GUI both depend on it.
add_subdirectory(qtk)
install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    COMPONENT libqtk
    DESTINATION lib/cmake/${PROJECT_NAME}
)
install(
    EXPORT qtk_export
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    COMPONENT libqtk
    DESTINATION lib/cmake/${PROJECT_NAME}
)
# System install for qtk_library
install(
    TARGETS qtk_library
    # Associate qtk_library target with qtk-export
    EXPORT qtk_export
    COMPONENT libqtk
    FILE_SET HEADERS DESTINATION include
    INCLUDES DESTINATION include
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    RUNTIME DESTINATION bin
)

# Qtk Application
if(QTK_BUILD_GUI OR QTK_INSTALL_PLUGINS)
  add_subdirectory(app)
endif()

if(QTK_INSTALL_PLUGINS)
  # Optionally install custom Qtk plugins for Qt Designer.
  install(
      TARGETS qtk_library qtk_plugin_library
      COMPONENT collection
      LIBRARY DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
      ARCHIVE DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
      RUNTIME DESTINATION "${QTK_PLUGIN_LIBRARY_DIR}"
  )
  install(
      TARGETS qtk_collection
      COMPONENT collection
      LIBRARY DESTINATION "${QTK_PLUGIN_INSTALL_DIR}"
      ARCHIVE DESTINATION "${QTK_PLUGIN_INSTALL_DIR}"
      RUNTIME DESTINATION "${QTK_PLUGIN_INSTALL_DIR}"
  )
endif()

if(QTK_BUILD_GUI)
  install(
      TARGETS qtk_app
      COMPONENT qtk
      BUNDLE DESTINATION .
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib/static
      RUNTIME DESTINATION bin
  )

  qt_generate_deploy_app_script(
      TARGET qtk_app
      OUTPUT_SCRIPT QTK_DEPLOY_SCRIPT
      NO_UNSUPPORTED_PLATFORM_ERROR
  )
  install(SCRIPT ${QTK_DEPLOY_SCRIPT} COMPONENT qtk)

  if(WIN32)
    if(MSVC AND TARGET Qt6::qmake)
      get_target_property(QT6_QMAKE_LOCATION Qt6::qmake IMPORTED_LOCATION)
      execute_process(
          COMMAND "${QT6_QMAKE_LOCATION}" -query QT_INSTALL_PREFIX
          RESULT_VARIABLE return_code
          OUTPUT_VARIABLE QT6_INSTALL_PREFIX
          OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      file(TO_NATIVE_PATH "${QT6_INSTALL_PREFIX}/bin" QT6_INSTALL_PREFIX)

      set(VSUSER_FILE "${CMAKE_CURRENT_BINARY_DIR}/qtk_app.vcxproj.user")
      file(WRITE ${VSUSER_FILE} "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
      file(APPEND ${VSUSER_FILE} "<Project xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n")
      file(APPEND ${VSUSER_FILE} "  <PropertyGroup>\n")
      file(APPEND ${VSUSER_FILE} "    <LocalDebuggerEnvironment>Path=$(SolutionDir)\\lib\\$(Configuration);${QT6_INSTALL_PREFIX};$(Path)\n")
      file(APPEND ${VSUSER_FILE} "$(LocalDebuggerEnvironment)</LocalDebuggerEnvironment>\n")
      file(APPEND ${VSUSER_FILE} "    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n")
      file(APPEND ${VSUSER_FILE} "  </PropertyGroup>\n")
      file(APPEND ${VSUSER_FILE} "</Project>\n")
    endif()
  endif()
endif()

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/templates/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VENDOR "Shaun Reed")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt OpenGL 3D graphics library.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/shaunrd0/qtk")
set(CPACK_SOURCE_IGNORE_FILES build*;install;\.git;\.github;\.idea)
set(CPACK_PACKAGE_DIRECTORY packages/)
set(CPACK_PACKAGE_CONTACT "shaunreed.com")
#set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/resources/icon.png")
set(CPACK_THREADS 0)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Qtk")

# Remove any assimp components if defined by submodule.
if (QTK_UPDATE_SUBMODULES)
  get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
  list(FILTER CPACK_COMPONENTS_ALL EXCLUDE REGEX .*assimp.*)
  list(REMOVE_ITEM CPACK_COMPONENTS_ALL Unspecified)
endif()

# Windows
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
# https://nsis.sourceforge.io/Reference/CreateShortCut
set(
    CPACK_NSIS_CREATE_ICONS_EXTRA
    "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Qtk.lnk' '$INSTDIR\\\\bin\\\\qtk_app.exe'"
)
set(
    CPACK_NSIS_DELETE_ICONS_EXTRA
    "Delete '$SMPROGRAMS\\\\$START_MENU\\\\Qtk.lnk'"
)
# TODO: Icons for NSIS installer.
#set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/resources/icon.png")
#set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/resources/icon.png")

# Debian
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CPACK_PACKAGE_HOMEPAGE_URL})
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# OSX
set(CPACK_BUNDLE_NAME ${PROJECT_NAME})
set(CPACK_BUNDLE_PLIST $<TARGET_BUNDLE_CONTENT_DIR:qtk_app>/Info.plist)
set(CPACK_BUNDLE_ICON ${QTK_OSX_ICONS})

# Platform defaults for source bundles.
if(WIN32)
  set(CPACK_SOURCE_GENERATOR ZIP)
else()
  set(CPACK_SOURCE_GENERATOR TGZ)
endif()

include(CPack)
