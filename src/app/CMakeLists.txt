################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2023 Shaun Reed, all rights reserved                       ##
################################################################################

################################################################################
# Qtk Widget Library
################################################################################
# Create a library of widgets used to build Qtk GUI
set(
    QTK_PLUGIN_LIBRARY_SOURCES
    qtkwidget.cpp
    debugconsole.cpp debugconsole.ui
    toolbox.cpp toolbox.ui
    treeview.cpp treeview.ui
    qtkmainwindow.cpp qtkmainwindow.h qtkmainwindow.ui
)
set(
    QTK_PLUGIN_LIBRARY_HEADERS
    qtkwidget.h
    debugconsole.h
    toolbox.h
    treeview.h
)
qt_add_library(qtk_plugin_library STATIC EXCLUDE_FROM_ALL)
target_sources(
    qtk_plugin_library PRIVATE
    "${QTK_PLUGIN_LIBRARY_SOURCES}"
    "${QTK_PLUGIN_LIBRARY_HEADERS}"
)
target_link_libraries(qtk_plugin_library PUBLIC Qt6::UiPlugin qtk_library)

################################################################################
# Qtk Widget Plugins
################################################################################
# Create a Qt Designer plugin for a collection of widgets from our library.
qt_add_plugin(qtk_plugins SHARED)
target_sources(
    qtk_plugins PRIVATE
    widgetplugincollection.cpp widgetplugincollection.h
    widgetplugin.cpp widgetplugin.h
)
target_link_libraries(qtk_plugins PUBLIC qtk_plugin_library)

################################################################################
# Final Qtk Application
################################################################################
# Source files for the main window and core application
set(
    QTK_GUI_SOURCES
    qtkscene.cpp qtkscene.h
#    logger.cpp logger.h
    main.cpp
)
qt_add_executable(qtk_gui ${QTK_GUI_SOURCES})
target_link_libraries(qtk_gui PRIVATE qtk_plugin_library)

set_target_properties(
    qtk_gui PROPERTIES
      WIN32_EXECUTABLE TRUE
      MACOSX_BUNDLE TRUE
      MACOSX_BUNDLE_BUNDLE_NAME Qtk
      MACOSX_BUNDLE_ICON_FILE ${QTK_OSX_ICONS}
      MACOSX_BUNDLE_GUI_IDENTIFIER ${CMAKE_PROJECT_NAME}
      MACOSX_BUNDLE_INFO_STRING ${CMAKE_PROJECT_DESCRIPTION}
      MACOSX_BUNDLE_COPYRIGHT "All Content (c) 2023 Shaun Reed, all rights reserved"
      MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
      MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)
