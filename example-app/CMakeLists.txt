################################################################################
## Example client project using qtk                                           ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2023 Shaun Reed, all rights reserved                       ##
################################################################################
cmake_minimum_required(VERSION 3.23)

################################################################################
# Constants
################################################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  add_compile_options(/wd4131 /wd4127)
endif()

# If you did not install Qtk on a system path, point cmake to installation.
set(
    QTK_PATH ../build/install/lib/cmake/Qtk
    CACHE PATH  "Path to installation of Qtk"
    FORCE
)

# If you did not install Qt6 on a system path, point cmake to installation.
set(QT_INSTALL_DIR "$ENV{HOME}/Qt/6.5.0/gcc_64/" CACHE PATH "Path to Qt6")

################################################################################
# Project
################################################################################
project(
    #[[NAME]]   QtkClient
    VERSION     0.1
    DESCRIPTION "An example project using Qtk"
    LANGUAGES   CXX C
)

list(APPEND CMAKE_PREFIX_PATH "${QTK_PATH}")
list(APPEND CMAKE_PREFIX_PATH "${QT_INSTALL_DIR}")

# Print all QTK variables
if (NOT Qtk_IS_TOP_LEVEL)
  get_cmake_property(VAR_NAMES VARIABLES)
  list(FILTER VAR_NAMES INCLUDE REGEX "^Q[tT][kK]_.*$")
  list(SORT VAR_NAMES)
  foreach(VAR_NAME ${VAR_NAMES})
    message(STATUS "[Qtk] ${VAR_NAME}=${${VAR_NAME}}")
  endforeach()
endif()

# Allow add_subdirectory on this project to use target ALIAS if available.
# If this example project is opened standalone we will use find_package.
if(NOT TARGET Qtk::qtk_library)
  find_package(Qtk 0.2 REQUIRED)
endif()

find_package(Qt6 COMPONENTS Core Widgets OpenGLWidgets REQUIRED)

set(
    EXAMPLE_SOURCES
    main.cpp
    examplescene.cpp examplescene.h
    examplewidget.cpp examplewidget.h
)

configure_file(
    #[[INPUT]] "${CMAKE_CURRENT_SOURCE_DIR}/resources.h.in"
    #[[OUTPUT]] "${CMAKE_CURRENT_BINARY_DIR}/resources.h"
    @ONLY
)

qt_add_executable(qtk_example ${EXAMPLE_SOURCES})
target_link_libraries(qtk_example PUBLIC Qt6::Widgets Qt6::OpenGLWidgets Qt6::Core)
target_link_libraries(qtk_example PUBLIC Qtk::qtk_library)
target_include_directories(qtk_example PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

install(
    TARGETS qtk_example
    COMPONENT qtk_example
    BUNDLE DESTINATION .
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    RUNTIME DESTINATION bin
)
qt_generate_deploy_app_script(
    TARGET qtk_example
    OUTPUT_SCRIPT QTK_EXAMPLE_DEPLOY_SCRIPT
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${QTK_EXAMPLE_DEPLOY_SCRIPT} COMPONENT qtk_example)
