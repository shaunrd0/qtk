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
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

#include <camera3d.h>
#include <mesh.h>
#include <qtkapi.h>
#include <texture.h>

namespace Qtk {
  /**
   * Skybox object for rendering a skybox within a Scene.
   * A skybox is typically implemented using a cube map texture centered around
   * the camera and projected outwards in all directions.
   */
  class QTKAPI Skybox : protected QOpenGLFunctions {
    public:
      /*************************************************************************
       * Constructors / Destructors
       ************************************************************************/

      // Delegate this constructor to use default skybox images
      explicit Skybox(const std::string & name = "Skybox");

      explicit Skybox(
          QOpenGLTexture * cubeMap, const std::string & name = "Skybox");

      Skybox(
          const std::string & right, const std::string & top,
          const std::string & front, const std::string & left,
          const std::string & bottom, const std::string & back,
          const std::string & name = "Skybox");

      ~Skybox() = default;

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      void draw();

    private:
      /*************************************************************************
       * Private Methods
       ************************************************************************/

      void init();

      /*************************************************************************
       * Private Members
       ************************************************************************/

      Vertices mVertices {};
      Indices mIndices {};

      QOpenGLShaderProgram mProgram;
      QOpenGLVertexArrayObject mVAO;
      QOpenGLBuffer mVBO;
      Texture mTexture;
  };
}  // namespace Qtk

#endif  // QTK_SKYBOX_H
