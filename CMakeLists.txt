################################################################################
## Author: Shaun Reed | Contact: shaunrd0@gmail.com | URL: www.shaunreed.com  ##
##                                                                            ##
## Project for working with OpenGL and Qt6 widgets                            ##
################################################################################
cmake_minimum_required(VERSION 3.2)

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
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(QTK_DEBUG "Enable debugger" ON)
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(ASSIMP_NEW_INTERFACE "Use assimp::assimp as target instead of assimp" OFF)

################################################################################
# External Libraries
################################################################################

# For CLion builds, point CMAKE_PREFIX_PATH to Qt6 install directory
list(APPEND CMAKE_PREFIX_PATH $ENV{HOME}/Code/Clones/Qt/6.3.1/gcc_64/)

# Find Qt
find_package(Qt6 COMPONENTS OpenGLWidgets)
if (NOT Qt6_FOUND)
  message(SEND_ERROR "Unable to find Qt6 at CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
  message(FATAL_ERROR "Specify path to Qt6 with `cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64 -S /path/to/qtk -B /path/to/qtk/build && cmake --build /path/to/qtk/build -j $(nprocs)`")
endif()

# Find Assimp
# https://github.com/assimp/assimp/commit/6ac8279977c3a54118551e549d77329497116f66
find_package(assimp REQUIRED)

include(GenerateExportHeader)

################################################################################
# Qtk
################################################################################

set(PUBLIC_HEADERS
    src/mainwidget.h
    src/abstractscene.h
    src/camera3d.h
    src/mesh.h
    src/meshrenderer.h
    src/model.h
    src/object.h
    src/skybox.h
    src/texture.h
    src/transform3D.h
)

set(SOURCE_FILES
    src/mainwidget.cpp
    src/abstractscene.cpp
    src/camera3d.cpp
    src/input.cpp
    src/input.h
    src/mesh.cpp
    src/meshrenderer.cpp
    src/model.cpp
    src/object.cpp
    src/qtkapi.h
    src/skybox.cpp
    src/texture.cpp
    src/transform3D.cpp
)
qt_add_library(qtk-widget STATIC ${PUBLIC_HEADERS} ${SOURCE_FILES})
target_include_directories(qtk-widget PRIVATE src/ app/)
generate_export_header(qtk-widget)

set_target_properties(qtk-widget PROPERTIES
    PUBLIC_HEADER "${PUBLIC_HEADERS}"
    VERSION ${PROJECT_VERSION}
)

target_include_directories(qtk-widget PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
if(BUILD_SHARED_LIBS)
  target_compile_definitions(qtk-widget PRIVATE QTK_EXPORT PUBLIC QTK_SHARED)
  target_link_libraries(qtk-widget PRIVATE Qt6::OpenGLWidgets)
  if(ASSIMP_NEW_INTERFACE)
    target_link_libraries(qtk-widget PUBLIC assimp::assimp)
  else()
    target_link_libraries(qtk-widget PUBLIC assimp)
  endif()
else()
  target_compile_definitions(qtk-widget PUBLIC QTK_STATIC)
  target_link_libraries(qtk-widget PUBLIC Qt6::OpenGLWidgets)
  if(ASSIMP_NEW_INTERFACE)
    target_link_libraries(qtk-widget PUBLIC assimp::assimp)
  else()
    target_link_libraries(qtk-widget PUBLIC assimp)
  endif()
endif()

if(QTK_DEBUG)
  target_compile_definitions(qtk-widget PUBLIC QTK_DEBUG)
endif()


if(WIN32)
  find_package(OpenGL REQUIRED)
  target_link_libraries(qtk-widget PUBLIC OpenGL::GL)
endif()

# Install files
install(TARGETS qtk-widget
    # Associate qtk-widget target with qtk-export
    EXPORT qtk-export
    # <prefix>/bin on DLL systems and <prefix>/lib on non-dll systems
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

# Install export
# qtkTargets.cmake will only be installed when one of the CONFIGURATIONS is installed
# + The generated import will only reference that qtk configuration
install(EXPORT qtk-export
    FILE qtkTargets.cmake
    CONFIGURATIONS Debug|Release
    DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
)

################################################################################
# Final Application
################################################################################

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/resources.h "#ifndef QTK_RESOURCES_H\n#define QTK_RESOURCES_H\n#define QTK_RESOURCES_DIR \"${CMAKE_SOURCE_DIR}/resources\"\n#endif\n")

# Add our Qt resources.qrc file to our application
set(QTK_APP_SOURCES app/main.cpp
    app/mainwindow.cpp app/mainwindow.h app/mainwindow.ui
    app/scene.cpp app/scene.h
    app/resourcemanager.cpp app/resourcemanager.h
    ${CMAKE_CURRENT_BINARY_DIR}/resources.h
)
qt6_add_big_resources(QTK_APP_SOURCES resources.qrc)

qt_add_executable(qtk-app ${QTK_APP_SOURCES})
target_include_directories(qtk-app PRIVATE src/ app/)

# Link qtk-app executable to main qtk-widget library
target_link_libraries(qtk-app PRIVATE Qt6::OpenGLWidgets qtk-widget)

set_target_properties(qtk-app PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)
install(TARGETS qtk-app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(WIN32)
  get_target_property(_qt6_qmake_location Qt6::qmake IMPORTED_LOCATION)
  execute_process(COMMAND "${_qt6_qmake_location}" -query QT_INSTALL_PREFIX RESULT_VARIABLE return_code OUTPUT_VARIABLE qt6_install_prefix OUTPUT_STRIP_TRAILING_WHITESPACE)
  file(TO_NATIVE_PATH "${qt6_install_prefix}/bin" qt6_install_prefix)
  if(TARGET Qt6::windeployqt)
    add_custom_command(TARGET qtk-app
        POST_BUILD
        COMMAND set PATH=%PATH%$<SEMICOLON>${qt6_install_prefix}
        COMMAND Qt6::windeployqt --dir "${CMAKE_BINARY_DIR}/windeployqt" "$<TARGET_FILE_DIR:qtk-app>/$<TARGET_FILE_NAME:qtk-app>"
        )
    install(DIRECTORY "${CMAKE_BINARY_DIR}/windeployqt/" DESTINATION bin)
  endif()
  if(MSVC AND TARGET Qt6::qmake)
    set(VSUSER_FILE ${CMAKE_CURRENT_BINARY_DIR}/qtk-app.vcxproj.user)
    file(TO_NATIVE_PATH "${assimp_DIR}/bin" assimp_bin)
    file(WRITE  ${VSUSER_FILE} "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
    file(APPEND ${VSUSER_FILE} "<Project xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n")
    file(APPEND ${VSUSER_FILE} "  <PropertyGroup>\n")
    file(APPEND ${VSUSER_FILE} "    <LocalDebuggerEnvironment>Path=$(SolutionDir)\\lib\\$(Configuration);${qt6_install_prefix};${assimp_bin};$(Path)\n")
    file(APPEND ${VSUSER_FILE} "$(LocalDebuggerEnvironment)</LocalDebuggerEnvironment>\n")
    file(APPEND ${VSUSER_FILE} "    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n")
    file(APPEND ${VSUSER_FILE} "  </PropertyGroup>\n")
    file(APPEND ${VSUSER_FILE} "</Project>\n")
  endif()
endif()
