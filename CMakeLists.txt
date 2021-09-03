################################################################################
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
##                                                                            ##
## Project for working with OpenGL and Qt5 widgets                            ##
################################################################################
cmake_minimum_required(VERSION 3.5)

project(
    #[[NAME]]   Qtk
    VERSION     1.0
    DESCRIPTION "An example project using QT and OpenGL"
    LANGUAGES   CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all Qt package components required for this project
find_package(Qt5 COMPONENTS Core LinguistTools Gui Widgets REQUIRED)

# Add our Qt resources.qrc file to our application
set(SOURCES app/main.cpp)
qt5_add_resources(SOURCES resources.qrc)

# Set translation files
set(TS_FILES qtk_en_US.ts)

add_executable(
    qtk             # Executable name
    ${SOURCES}      # Executable source code
    ${TS_FILES}     # Link translation files
)


################################################################################
# External Libraries
################################################################################

# Find and link GLUT package; Otherwise show an error
find_package(GLUT REQUIRED)
if (!GLUT_FOUND)
  message(
      "Error: CMake was unable to find the GLUT package\n"
      "Please install GLUT (freeglut3-dev) and try again\n"
      "sudo apt install freeglut3-dev\n"
  )
endif()

# Find and link OpenGL package; Otherwise show an error
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
if (!OPENGL_FOUND)
  message(
      "Error: CMake was unable to find the OpenGL package\n"
      "Please install OpenGL and try again\n"
      "sudo apt install mesa-utils\n"
  )
endif()

# https://github.com/assimp/assimp/commit/6ac8279977c3a54118551e549d77329497116f66
find_package(assimp REQUIRED)
if (!assimp_FOUND)
  message(
      "Error: CMake was unable to find the Assimp package\n"
      "Please install Assimp and try again\n"
      "sudo apt install libassimp-dev\n"
  )
endif()


################################################################################
# Custom Libraries
################################################################################

# Mainwidget
add_library(main-widget lib/mainwidget.cpp)
target_include_directories(main-widget PUBLIC lib/)
#target_link_libraries(main-widget PUBLIC Qt5::Widgets)
# + This lib and all linked targets will also link to OpenGL
target_include_directories(main-widget PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(main-widget PUBLIC ${OPENGL_LIBRARIES})

# Model
add_library(model lib/model.cpp)
target_include_directories(model PRIVATE ${ASSIMP_INCLUDE_DIR})
target_link_libraries(model PRIVATE ${ASSIMP_LIBRARIES})
target_link_libraries(model PUBLIC Qt5::Widgets)
target_link_libraries(model PRIVATE main-widget)

# Input
add_library(input lib/input.cpp)
target_include_directories(input PUBLIC lib/)
target_link_libraries(input PUBLIC Qt5::Widgets)

# Mesh
add_library(mesh lib/mesh.cpp)
target_include_directories(mesh PUBLIC lib/)
target_link_libraries(mesh PUBLIC Qt5::Widgets)

# Transform3D
add_library(transform3d lib/transform3D.cpp)
target_include_directories(transform3d PUBLIC lib/)
target_link_libraries(transform3d PUBLIC Qt5::Widgets)

# Camera3D
add_library(camera3d lib/camera3d.cpp)
target_include_directories(camera3d PUBLIC lib/)
target_link_libraries(camera3d PUBLIC Qt5::Widgets)

# Texture
add_library(texture lib/texture.cpp)
target_include_directories(texture PUBLIC lib/)
target_link_libraries(texture PUBLIC Qt5::Widgets)

# Object
add_library(object lib/object.cpp)
target_include_directories(object PUBLIC lib/)
target_link_libraries(object PUBLIC Qt5::Widgets)

# MeshRenderer
add_library(meshrenderer lib/meshrenderer.cpp)
target_include_directories(meshrenderer PUBLIC lib/)
target_link_libraries(meshrenderer PUBLIC Qt5::Widgets)

# Skybox
add_library(skybox lib/skybox.cpp)
target_link_libraries(skybox PUBLIC Qt5::Widgets)
target_link_libraries(skybox PRIVATE mesh)
target_link_libraries(skybox PRIVATE camera3d)

################################################################################
# Final Application
################################################################################

target_link_libraries(main-widget PUBLIC model)
target_link_libraries(main-widget PUBLIC input)
target_link_libraries(main-widget PUBLIC transform3d)
target_link_libraries(main-widget PUBLIC object)
target_link_libraries(main-widget PUBLIC meshrenderer)
target_link_libraries(main-widget PUBLIC texture)
target_link_libraries(main-widget PUBLIC skybox)
target_link_libraries(main-widget PUBLIC mesh)

# Link qtk executable to main main-widget library
target_link_libraries(qtk PUBLIC main-widget)

# Set up QT Linguist translation
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
