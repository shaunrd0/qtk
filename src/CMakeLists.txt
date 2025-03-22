################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2025 Shaun Reed, all rights reserved                       ##
################################################################################

# Qtk Library

# We always build libqtk since the plugins and GUI both depend on it.
add_subdirectory(qtk)
install(
    FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    COMPONENT qtk
    DESTINATION lib/cmake/${PROJECT_NAME}
)
install(
    EXPORT qtk_export
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    COMPONENT qtk
    DESTINATION lib/cmake/${PROJECT_NAME}
)
# System install for libqtk
install(
    TARGETS qtk
    # Associate libqtk target with qtk-export
    EXPORT qtk_export
    COMPONENT qtk
    FILE_SET HEADERS DESTINATION include
    INCLUDES DESTINATION include
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# The Qtk Application's GUI requires plugins to be built, but not installed.
# If both are disabled we can skip. If QTK_PLUGINS is set we'll install the
# plugins to Qt Designer for use in other projects.
if(QTK_PLUGINS OR QTK_GUI)
  add_subdirectory(designer-plugins)
endif()

# Build Qtk Application only if QTK_GUI is set.
if(QTK_GUI)
  add_subdirectory(app)
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
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VENDOR "Shaun Reed")
if (QTK_GUI)
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt OpenGL 3D graphics renderer.")
else()
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Qt OpenGL 3D graphics library.")
endif()
set(CPACK_PACKAGE_HOMEPAGE_URL "https://git.shaunreed.com/shaunrd0/qtk")
set(CPACK_SOURCE_IGNORE_FILES build*;install;\.git;\.github;\.idea)
set(CPACK_PACKAGE_DIRECTORY packages/)
set(CPACK_PACKAGE_CONTACT "shaunreed.com")
set(CPACK_THREADS 0)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Qtk")

# Remove any assimp components if defined by submodule.
if (QTK_SUBMODULES)
  get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
  list(FILTER CPACK_COMPONENTS_ALL EXCLUDE REGEX .*assimp.*)
  list(REMOVE_ITEM CPACK_COMPONENTS_ALL Unspecified)
endif()

if(WIN32)
  # Windows
  # Set icon for the splash image used in Windows NSIS installers.
  # TODO: Fix NSIS cmake error
  # File: "D:/a/qtk/qtk/resources/icons/win/kilroy_splash.bmp" -> no files found.
  #set(CPACK_PACKAGE_ICON "${QTK_RESOURCES}/icons/win/kilroy_splash.bmp")
  set(CPACK_NSIS_MODIFY_PATH ON)
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
  # https://nsis.sourceforge.io/Reference/CreateShortCut
  # TODO: Use 4th parameter to pass icon.
  set(
      CPACK_NSIS_CREATE_ICONS_EXTRA
      "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Qtk.lnk' '$INSTDIR\\\\bin\\\\qtk_gui.exe'\n\
       CreateShortCut '$DESKTOP\\\\Qtk.lnk' '$INSTDIR\\\\bin\\\\qtk_gui.exe'"
  )
  set(
      CPACK_NSIS_DELETE_ICONS_EXTRA
      "Delete '$SMPROGRAMS\\\\$START_MENU\\\\Qtk.lnk'\n\
       Delete '$DESKTOP\\\\Qtk.lnk'"
  )

  # Set icon for the installer executable.
  set(CPACK_NSIS_MUI_ICON "${QTK_RESOURCES}/icons/win/kilroy.ico")
  # Set icon for uninstaller executable to be the same as the installer.
  set(CPACK_NSIS_MUI_UNIICON "${CPACK_NSIS_MUI_ICON}")
elseif(APPLE)
  # OSX
  # Set icon for the splash image used in OSX DMG installers.
  set(CPACK_PACKAGE_ICON "${QTK_RESOURCES}/icons/osx/kilroy_splash.bmp")

  # Set additional options for the OSX bundle.
  set(CPACK_BUNDLE_NAME ${PROJECT_NAME})
  set(CPACK_BUNDLE_PLIST $<TARGET_BUNDLE_CONTENT_DIR:qtk_gui>/Info.plist)
  set(CPACK_BUNDLE_ICON "${QTK_OSX_ICONS}")
else(UNIX)
  # Linux
  # Install icons for qtk.
  install(FILES "${QTK_RESOURCES}/icons/linux/kilroy-16.png"
      DESTINATION /usr/share/icons/hicolor/16x16/apps
      RENAME qtk.png)
  install(FILES "${QTK_RESOURCES}/icons/linux/kilroy-32.png"
      DESTINATION /usr/share/icons/hicolor/32x32/apps
      RENAME qtk.png)
  install(FILES "${QTK_RESOURCES}/icons/linux/kilroy-48.png"
      DESTINATION /usr/share/icons/hicolor/48x48/apps
      RENAME qtk.png)
  install(FILES "${QTK_RESOURCES}/icons/linux/kilroy-64.png"
      DESTINATION /usr/share/icons/hicolor/64x64/apps
      RENAME qtk.png)
  install(FILES "${QTK_RESOURCES}/icons/linux/kilroy-128.png"
      DESTINATION /usr/share/icons/hicolor/128x128/apps
      RENAME qtk.png)
  install(FILES "${QTK_RESOURCES}/icons/linux/kilroy-256.png"
      DESTINATION /usr/share/icons/hicolor/256x256/apps
      RENAME qtk.png)

  # Install the desktop file for qtk.
  install(FILES "${QTK_RESOURCES}/qtk.desktop"
      DESTINATION /usr/share/applications)

  # Set additional options for the debian APT package.
  set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CPACK_PACKAGE_HOMEPAGE_URL})
  set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
endif()

if(WIN32)
  set(CPACK_SOURCE_GENERATOR ZIP)
else()
  set(CPACK_SOURCE_GENERATOR TGZ)
endif()

include(CPack)
