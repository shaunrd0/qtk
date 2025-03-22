################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2025 Shaun Reed, all rights reserved                       ##
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
target_link_libraries(qtk_plugin_library PUBLIC Qt6::UiPlugin qtk)

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

# Install the Qt Designer plugins only if QTK_PLUGINS is set.
# Otherwise, we just use them for building the Qtk desktop application.
if(QTK_PLUGINS)
  install(
      TARGETS qtk_plugins qtk qtk_plugin_library
      COMPONENT qtk_plugins
      LIBRARY DESTINATION "${QTK_PLUGIN_INSTALL_DIR}"
      ARCHIVE DESTINATION "${QTK_PLUGIN_INSTALL_DIR}"
      RUNTIME DESTINATION "${QTK_PLUGIN_INSTALL_DIR}"
  )
endif()