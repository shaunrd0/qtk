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
add_library(main-widget lib/mainwidget.cpp)
target_include_directories(main-widget PUBLIC lib/)
target_include_directories(main-widget PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(main-widget PRIVATE ${OPENGL_LIBRARIES})

# Input
add_library(input lib/input.cpp)
target_include_directories(input PUBLIC lib/)
target_link_libraries(input PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

# Mesh
add_library(mesh lib/mesh.cpp)
target_include_directories(mesh PUBLIC lib/)
target_link_libraries(mesh PUBLIC Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

# Texture
add_library(texture lib/texture.cpp)
target_include_directories(texture PUBLIC lib/)
target_link_libraries(texture PUBLIC Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

# Object
add_library(object lib/object.cpp)
target_include_directories(object PUBLIC lib/)
target_link_libraries(object INTERFACE mesh)
target_link_libraries(object PUBLIC Qt${QT_VERSION_MAJOR}::OpenGL)
target_link_libraries(object PUBLIC Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

# MeshRenderer
add_library(meshrenderer lib/meshrenderer.cpp)
target_include_directories(meshrenderer PUBLIC lib/)
target_link_libraries(meshrenderer PUBLIC object)

# Camera3D
add_library(camera3d lib/camera3d.cpp)
target_include_directories(camera3d PUBLIC lib/)
target_link_libraries(camera3d INTERFACE input)
target_link_libraries(camera3d PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

# Skybox
add_library(skybox lib/skybox.cpp)
# Skybox needs Mesh, Camera3D, and Qt6::Widgets
target_link_libraries(skybox PRIVATE mesh)
target_link_libraries(skybox PRIVATE camera3d)
target_link_libraries(skybox PUBLIC Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

# Transform3D
add_library(transform3d lib/transform3D.cpp)
target_include_directories(transform3d PUBLIC lib/)
target_link_libraries(transform3d PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

# Model
add_library(model lib/model.cpp)
target_include_directories(model PUBLIC lib/)
target_link_libraries(model PRIVATE assimp)
target_link_libraries(model PUBLIC Qt${QT_VERSION_MAJOR}::OpenGL)
# Model library requires transform3d and Qt6::Widgets
target_link_libraries(model PUBLIC transform3d)
target_link_libraries(model PUBLIC Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

# Scene
add_library(scene lib/scene.cpp)
target_include_directories(scene PUBLIC lib/)
target_link_libraries(scene PUBLIC model)
target_link_libraries(scene PUBLIC meshrenderer)
target_link_libraries(scene PUBLIC skybox)
target_link_libraries(scene PUBLIC texture)
target_link_libraries(scene PUBLIC Qt${QT_VERSION_MAJOR}::OpenGLWidgets)


################################################################################
# Final Application
################################################################################

target_link_libraries(main-widget PRIVATE mesh)
target_link_libraries(main-widget PUBLIC scene)

# Link qtk executable to main main-widget library
target_link_libraries(qtk PUBLIC main-widget)
