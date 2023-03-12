/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
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

#include "camera3d.h"
#include "qtkapi.h"
#include "shape.h"
#include "texture.h"

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

      /**
       * Construct Skybox using default images.
       *
       * @param name The objectName to use for the Skybox.
       */
      explicit Skybox(const std::string & name = "Skybox");

      /**
       * Construct a skybox with an existing QOpenGLTexture.
       * The texture should be a fully initialized cube map.
       *
       * @param cubeMap QOpenGLTexture to use for the new Skybox.
       * @param name The objectName to use for the Skybox.
       */
      explicit Skybox(
          QOpenGLTexture * cubeMap, const std::string & name = "Skybox");

      /**
       * Construct a Skybox.
       *
       * @param right Image to use for the right side of the Skybox.
       * @param top Image to use for the top side of the Skybox.
       * @param front Image to use for the front side of the Skybox.
       * @param left Image to use for the left side of the Skybox.
       * @param bottom Image to use for the bottom side of the Skybox.
       * @param back Image to use for the back side of the Skybox.
       * @param name The objectName to use for this Skybox.
       */
      Skybox(
          const std::string & right, const std::string & top,
          const std::string & front, const std::string & left,
          const std::string & bottom, const std::string & back,
          const std::string & name = "Skybox");

      ~Skybox() = default;

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      /**
       * Draws the skybox.
       */
      void draw();

    private:
      /*************************************************************************
       * Private Methods
       ************************************************************************/

      /**
       * Initializes OpenGL buffers and shaders for this skybox.
       */
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
