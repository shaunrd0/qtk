/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Skybox class using QtOpenGL                                         ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_SKYBOX_H
#define QTK_SKYBOX_H

#include <QImage>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

#include <camera3d.h>
#include <mesh.h>


class Skybox : protected QOpenGLFunctions {
public:
  // Delegate this constructor to use default skybox images
  // + This allows creating a skybox with no arguments ( auto s = new Skybox; )
  explicit Skybox(std::string name="Skybox");
  explicit Skybox(QOpenGLTexture * cubeMap, const std::string & name="Skybox");
  // Constructor, Destructor
  Skybox(std::string right, std::string top, std::string front,
         std::string left, std::string bottom, std::string back,
         const std::string & name="Skybox");
  ~Skybox() {}

  void draw();

private:
  void init();

  Vertices mVertices;
  Indices mIndices;

  QOpenGLShaderProgram mProgram;
  QOpenGLVertexArrayObject mVAO;
  QOpenGLBuffer mVBO;
  QOpenGLTexture * mCubeMap;
};

#endif // QTK_SKYBOX_H
