################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2025 Shaun Reed, all rights reserved                       ##
################################################################################

################################################################################
# Qtk Application
################################################################################

# Build without the example scene if QTK_GUI_SCENE is unset.
if (QTK_GUI_SCENE)
  set(QTK_GUI_SOURCES
      qtkmainwindow.cpp qtkmainwindow.h qtkmainwindow.ui
      # This scene will clone a git repository for required 3D models.
      qtkscene.cpp qtkscene.h
      main.cpp
  )

  qt6_add_big_resources(QTK_GUI_SOURCES "${QTK_RESOURCES}/resources.qrc")
else()
  # The scene will use a default skybox with no models or examples.
  # Models can be added by click-and-dragging an .obj into the scene.
  set(QTK_GUI_SOURCES
      qtkmainwindow.cpp qtkmainwindow.h qtkmainwindow.ui
      main.cpp
  )

  qt6_add_big_resources(
      QTK_GUI_SOURCES
      "${QTK_RESOURCES}/minimal_resources.qrc"
  )
endif()

qt_add_executable(qtk_gui ${QTK_GUI_SOURCES})
target_link_libraries(qtk_gui PRIVATE qtk_plugin_library)

if (QTK_GUI_SCENE)
  target_compile_definitions(qtk_gui PRIVATE -DQTK_GUI_SCENE)
endif()

if (WIN32)
  set_target_properties(qtk_gui PROPERTIES WIN32_EXECUTABLE TRUE)
elseif(APPLE)
  set_target_properties(
      qtk_gui PROPERTIES
      MACOSX_BUNDLE TRUE
      MACOSX_BUNDLE_BUNDLE_NAME Qtk
      MACOSX_BUNDLE_ICON_FILE "${QTK_OSX_ICONS}"
      MACOSX_BUNDLE_GUI_IDENTIFIER ${CMAKE_PROJECT_NAME}
      MACOSX_BUNDLE_INFO_STRING ${CMAKE_PROJECT_DESCRIPTION}
      MACOSX_BUNDLE_COPYRIGHT ${QTK_COPYRIGHT}
      MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
      MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  )
endif()

# qt_generate_deploy_app_script supports Windows and OSX in QtCore >= 6.3.
# qt_generate_deploy_app_script is supported on Linux in QtCore >= 6.5.0.
if((Qt6_VERSION VERSION_GREATER_EQUAL "6.3.0" AND (WIN32 OR APPLE))
    OR Qt6_VERSION VERSION_GREATER_EQUAL "6.5.0")
  install(
      TARGETS qtk_gui
      COMPONENT qtk_gui
      BUNDLE DESTINATION .
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
      RUNTIME DESTINATION bin
  )
  qt_generate_deploy_app_script(
      TARGET qtk_gui
      OUTPUT_SCRIPT QTK_DEPLOY_SCRIPT
      NO_UNSUPPORTED_PLATFORM_ERROR
  )
  install(SCRIPT ${QTK_DEPLOY_SCRIPT} COMPONENT qtk_gui)
endif()

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

    set(VSUSER_FILE "${CMAKE_CURRENT_BINARY_DIR}/qtk_gui.vcxproj.user")
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