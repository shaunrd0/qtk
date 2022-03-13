################################################################################
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
##                                                                            ##
## Project for working with OpenGL and Qt6 widgets                            ##
################################################################################
cmake_minimum_required(VERSION 3.5)

project(
    #[[NAME]]   Qtk
    VERSION     1.0
    DESCRIPTION "An example project using QT and OpenGL"
    LANGUAGES   CXX C
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Widgets OpenGLWidgets OpenGL REQUIRED)

# Add our Qt resources.qrc file to our application
set(SOURCES app/main.cpp)
qt6_add_resources(SOURCES resources.qrc)

add_executable(
    qtk             # Executable name
    ${SOURCES}      # Executable source code
)

################################################################################
# External Libraries
################################################################################

# Find and link OpenGL package; Otherwise show an error
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)

# https://github.com/assimp/assimp/commit/6ac8279977c3a54118551e549d77329497116f66
find_package(assimp REQUIRED)


################################################################################
# Custom Libraries
################################################################################

# Mainwidget
include(GenerateExportHeader)
add_library(main-widget SHARED
    src/mainwidget.cpp src/mainwidget.h
    src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
    src/input.cpp src/input.h
    src/mesh.cpp src/mesh.h
    src/texture.cpp src/texture.h
    src/object.cpp src/object.h
    src/meshrenderer.cpp src/meshrenderer.h
    src/camera3d.cpp src/camera3d.h
    src/skybox.cpp src/skybox.h
    src/transform3D.cpp src/transform3D.h
    src/model.cpp src/model.h
    src/scene.cpp src/scene.h
)

target_include_directories(main-widget PUBLIC src/)
target_include_directories(main-widget PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(main-widget PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(main-widget PRIVATE assimp)
target_link_libraries(main-widget PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(main-widget PUBLIC Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

################################################################################
# Final Application
################################################################################

# Link qtk executable to main main-widget library
target_link_libraries(qtk PUBLIC main-widget)

set_target_properties(qtk PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

generate_export_header(main-widget)
