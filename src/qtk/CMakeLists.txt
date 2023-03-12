################################################################################
## Project for working with OpenGL and Qt6 widgets                            ##
##                                                                            ##
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
## All Content (c) 2023 Shaun Reed, all rights reserved                       ##
################################################################################

################################################################################
# Qtk Library
################################################################################
set(
    QTK_LIBRARY_PUBLIC_HEADERS
    camera3d.h
    input.h
    meshrenderer.h
    model.h
    modelmesh.h
    object.h
    qtkapi.h
    qtkiostream.h
    qtkiosystem.h
    scene.h
    shape.h
    skybox.h
    texture.h
    transform3D.h
)

set(
    QTK_LIBRARY_SOURCES
    camera3d.cpp
    input.cpp
    meshrenderer.cpp
    model.cpp
    modelmesh.cpp
    object.cpp
    qtkiostream.cpp
    qtkiosystem.cpp
    scene.cpp
    shape.cpp
    skybox.cpp
    texture.cpp
    transform3D.cpp
)

qt6_add_big_resources(QTK_LIBRARY_SOURCES "${QTK_RESOURCES}/resources.qrc")
qt_add_library(qtk_library STATIC EXCLUDE_FROM_ALL)
target_sources(qtk_library PRIVATE ${QTK_LIBRARY_SOURCES})
target_sources(
    qtk_library PUBLIC
    FILE_SET HEADERS
      BASE_DIRS $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
      BASE_DIRS $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
      FILES ${QTK_LIBRARY_PUBLIC_HEADERS}
)

if(QTK_DEBUG)
  target_compile_definitions(qtk_library PUBLIC QTK_DEBUG)
endif()

set_target_properties(
    qtk_library PROPERTIES
      VERSION ${PROJECT_VERSION}
)

target_link_libraries(
    qtk_library PUBLIC
    Qt6::Core Qt6::OpenGLWidgets Qt6::Widgets
)

if(QTK_UPDATE_SUBMODULES OR NOT ASSIMP_NEW_INTERFACE)
  target_link_libraries(qtk_library PUBLIC assimp)
elseif(ASSIMP_NEW_INTERFACE)
  target_link_libraries(qtk_library PUBLIC assimp::assimp)
endif()

if(WIN32)
  target_link_libraries(qtk_library PUBLIC OpenGL::GL)
endif()
