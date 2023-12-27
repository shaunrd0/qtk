################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2023 Shaun Reed, all rights reserved                       ##
################################################################################
cmake_minimum_required(VERSION 3.23)

################################################################################
# Constants
################################################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MACOSX_BUNDLE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
  set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)
  add_compile_options(/wd4131 /wd4127)
endif()
add_compile_options(-fPIC)

################################################################################
# Project
################################################################################
project(
    #[[NAME]]   Qtk
    VERSION     0.2
    DESCRIPTION "Qt OpenGL library and desktop application."
    LANGUAGES   CXX C
)

################################################################################
# Includes
################################################################################
include("${CMAKE_SOURCE_DIR}/cmake/include/git_submodule.cmake")
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

################################################################################
# Options
################################################################################
option(QTK_DEBUG "Enable debugger" OFF)
option(QTK_SUBMODULES "Update external project (assimp) submodule" OFF)
option(QTK_GUI "Build the Qtk desktop application" ON)
option(QTK_PLUGINS "Install Qtk plugins to Qt Creator path." OFF)
option(QTK_EXAMPLE "Build the Qtk example desktop application" ON)
option(QTK_CCACHE "Enable ccache" ON)

if (QTK_CCACHE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif()

# Install Qtk for use within Qt Creator projects only, instead of system-wide.
option(QTK_PREFIX_QTCREATOR "Install Qtk to Qt Creator. Untested." OFF)

# Option for bringing your own assimp installation; Otherwise not needed
# + If assimp is available system-wide we can just set QTK_SUBMODULES OFF
option(
    QTK_ASSIMP_NEW_INTERFACE
    "Use the assimp::assimp interface (WIN / OSX)"
    OFF
)

if(QTK_DEBUG OR CMAKE_BUILD_TYPE MATCHES "^[Dd][Ee][Bb][Uu][Gg]$")
  set(QTK_DEBUG ON)
  set(CMAKE_BUILD_TYPE Debug)
else()
  set(QTK_DEBUG OFF)
  set(CMAKE_BUILD_TYPE Release)
endif()

# This should be set to your Qt6 installation directory.
set(QT_INSTALL_DIR "$ENV{HOME}/Qt/6.5.0/gcc_64/lib/cmake" CACHE PATH "Path to Qt6 install.")
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX  "${CMAKE_BINARY_DIR}/install")
endif ()

set(QTK_RESOURCES "${CMAKE_SOURCE_DIR}/resources")
set(QTK_OSX_ICONS ${CMAKE_SOURCE_DIR}/resources/icons/osx/kilroy.icns)

# Point CMAKE_PREFIX_PATH to Qt6 install directory
# If Qtk is built within Qt Creator this is not required.
list(APPEND CMAKE_PREFIX_PATH "${QT_INSTALL_DIR}")
if (QTK_PREFIX_QTCREATOR)
  # TODO: This might be a bit strange and needs more testing.
  set(CMAKE_INSTALL_PREFIX  "${QT_INSTALL_DIR}")
endif()

set(
    QT_CREATOR_DIR
    "${QT_INSTALL_DIR}/../../Tools/QtCreator"
    CACHE PATH "Qt Creator path used to install Qtk plugins for Qt Designer."
)

# Print all QTK options and their values at the end of configuration.
# We initialize this list here so that we can append to it as needed.
# All variables in this list will be printed at the end of configuration.
get_cmake_property(VAR_NAMES VARIABLES)
list(FILTER VAR_NAMES INCLUDE REGEX "^[qQ][tT][kK]_.*$")
list(SORT VAR_NAMES)

################################################################################
# External Dependencies
################################################################################
# Find Qt
find_package(Qt6 COMPONENTS Core UiPlugin OpenGLWidgets)
qt_standard_project_setup()
if(NOT Qt6_FOUND)
  message(
      SEND_ERROR "[Qtk] Error: Unable to find Qt6 at CMAKE_PREFIX_PATH: "
      "${CMAKE_PREFIX_PATH}"
  )
  message(
      FATAL_ERROR "[Qtk] Error: Specify path to Qt6 with `cmake "
      "-DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64 -S /path/to/qtk -B "
      "/path/to/qtk/build && cmake --build /path/to/qtk/build -j $(nprocs)`"
  )
endif()

#
# To use custom plugins, set QT_PLUGIN_PATH environment variable before running designer
# Or, we can install plugins to the designer for use across all projects.
# Qt Creator on linux will look here for widget plugins in the integrated designer
#   /home/shaun/Qt/Tools/QtCreator/lib/Qt/lib
# Qt Designer will use the following path on linux
#   /home/shaun/Qt/6.5.0/gcc_64/plugins/designer/
# We can use this path after find_package(Qt6) to install our plugins on all systems
#   ${QT6_INSTALL_PREFIX}/${QT6_INSTALL_PLUGINS}/designer
# And run designer at ${QT6_INSTALL_PREFIX}/bin/designer
# Use cmake -DQTK_PLUGIN_INSTALL_DIR=/some/path to override this install path
set(
    QTK_PLUGIN_INSTALL_DIR
    "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_PLUGINS}/designer" CACHE PATH
    "Path to install Qtk plugin collection."
)
# See cmake configure output for values of these variables on your system
set(
  VAR_PATHS
   CMAKE_PREFIX_PATH CMAKE_INSTALL_PREFIX QTK_PLUGIN_INSTALL_DIR QT6_INSTALL_PREFIX
   QT_INSTALL_DIR
)
# Add QT6_INSTALL_PLUGINS to VAR_NAMES so it is printed at end of configuration.
list(APPEND VAR_NAMES QT6_INSTALL_PLUGINS)

# Find Assimp.
if(QTK_SUBMODULES)
  # Required to statically link.
  add_compile_options(-fPIC)
  set(BUILD_SHARED_LIBS OFF CACHE STRING "Build static assimp libs" FORCE)
  set(ASSIMP_BUILD_ZLIB ON CACHE STRING "Build Zlib with assimp." FORCE)
  set(
      ASSIMP_INSTALL
      OFF CACHE STRING "Disable to use assimp as a submodule."
      FORCE
  )
  set(ASSIMP_NO_EXPORT ON CACHE STRING "Disable to export assimp." FORCE)
  set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE STRING "No warnings as errors." FORCE)
  set(ASSIMP_BUILD_TESTS OFF CACHE STRING "Do not build assimp tests." FORCE)
  message(STATUS "[Qtk] Updating submodules...")
  submodule_update("${CMAKE_CURRENT_SOURCE_DIR}/extern/assimp/assimp/")
  add_subdirectory(
      "${CMAKE_CURRENT_SOURCE_DIR}/extern/assimp/assimp/"
      EXCLUDE_FROM_ALL
  )
else()
  find_package(assimp REQUIRED)
endif()

if(WIN32)
  find_package(OpenGL REQUIRED)
endif()

################################################################################
# Qtk
################################################################################
add_subdirectory(src)

if(QTK_EXAMPLE)
  # Create a namespaced alias for linking with qtk_library in the example.
  add_library(${PROJECT_NAME}::qtk_library ALIAS qtk_library)
  add_subdirectory(example-app EXCLUDE_FROM_ALL)
endif()

# Print all QTK options and their values at the end of configuration. This also
# prints any additional variables that we have added to VAR_NAMES and VAR_PATHS.
foreach(VAR_NAME IN LISTS VAR_NAMES VAR_PATHS)
  if(VAR_NAME IN_LIST VAR_PATHS)
    # Print absolute if variable is path
    get_filename_component(VAR_REALPATH "${${VAR_NAME}}" REALPATH)
    message(STATUS "[Qtk] ${VAR_NAME}=${VAR_REALPATH}")
  else()
    message(STATUS "[Qtk] ${VAR_NAME}=${${VAR_NAME}}")
  endif()
endforeach()
