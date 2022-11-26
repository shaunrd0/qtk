/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Texture class to help with texture and image initializations        ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTOPENGL_TEXTURE_H
#define QTOPENGL_TEXTURE_H

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <utility>

#include <qtkapi.h>

namespace Qtk {
  /**
   * Binds shader programs until the end of scope.
   * Does nothing if the shader program was already bound.
   *
   * See MeshRenderer::setUniform() for example.
   */
  class QTKAPI ShaderBindScope {
    public:
      /*************************************************************************
       * Constructors / Destructors
       ************************************************************************/

      explicit ShaderBindScope(
          QOpenGLShaderProgram * program, bool was_locked) :
          mWasBound(was_locked) {
        mProgram = program;
        if(!mWasBound) {
          mProgram->bind();
        }
      }

      ~ShaderBindScope() {
        if(!mWasBound) {
          mProgram->release();
        }
      }

    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/

      QOpenGLShaderProgram * mProgram;
      bool mWasBound;
  };

  /**
   * Factories for initializing various OpenGL textures
   */
  class QTKAPI OpenGLTextureFactory {
    public:
      /*************************************************************************
       * Constructors / Destructors
       ************************************************************************/
      ~OpenGLTextureFactory() = default;

      /*************************************************************************
       * Texture Factories
       ************************************************************************/

      /**
       * QImage factory
       *
       * @param image Path to image we want to load.
       *    Can be absolute or Qt resource path.
       * @param flipX If true the image will be flipped on X axis.
       * @param flipY If true the image will be flipped on Y axis.
       * @return Pointer to an initialized QImage object.
       */
      static QImage * initImage(
          const char * image, bool flipX = false, bool flipY = false);

      /**
       * QOpenGLTexture factory
       *
       * @param texture Path to texture we want to load.
       *    Can be absolute or Qt resource path.
       * @param flipX If true the image will be flipped on X axis.
       * @param flipY If true the image will be flipped on Y axis.
       * @return Pointer to an initialized QOpenGLTexture object.
       */
      static QOpenGLTexture * initTexture(
          const char * texture, bool flipX = false, bool flipY = false);

      /**
       * Cube map factory for initializing all sides of a CubeMap.
       * All of these parameters can be absolute or Qt resource paths.
       *
       * @param right Path to image for the right side of the CubeMap.
       * @param top Path to image for the top side of the CubeMap.
       * @param front Path to image for the front side of the CubeMap.
       * @param left Path to image for the left side of the CubeMap.
       * @param bottom Path to image for the bottom side of the CubeMap.
       * @param back Path to image for the back side of the CubeMap.
       * @return Pointer to an initialized QOpenGLTexture object.
       */
      static QOpenGLTexture * initCubeMap(
          const QImage & right, const QImage & top, const QImage & front,
          const QImage & left, const QImage & bottom, const QImage & back);

      /**
       * CubeMap factory for tiling the same image on all sides.
       *
       * @param tile Path to the image we want to make into a CubeMap.
       *    Can be absolute or Qt resource path.
       * @return Pointer to an initialized QOpenGLTexture object.
       */
      static QOpenGLTexture * initCubeMap(const char * tile);

      /**
       * Cube map factory for initializing all sides of a CubeMap.
       * All of these parameters can be absolute or Qt resource paths.
       *
       * @param right Path to image for the right side of the CubeMap.
       * @param top Path to image for the top side of the CubeMap.
       * @param front Path to image for the front side of the CubeMap.
       * @param left Path to image for the left side of the CubeMap.
       * @param bottom Path to image for the bottom side of the CubeMap.
       * @param back Path to image for the back side of the CubeMap.
       * @return Pointer to an initialized QOpenGLTexture object.
       */
      static QOpenGLTexture * initCubeMap(
          const char * right, const char * top, const char * front,
          const char * left, const char * bottom, const char * back);

    private:
      // Private ctor to prevent creating instances of this class
      OpenGLTextureFactory() = default;
  };

  /**
   * Texture object component class
   *
   * TODO: Struct for (re)storing texture state
   * A struct to store flipX, flipY and other initial state needed to copy
   */
  class Texture {
    public:
      /*************************************************************************
       * Typedefs
       ************************************************************************/
      friend class Skybox;

      /*************************************************************************
       * Constructors / Destructors
       ************************************************************************/

      Texture() = default;

      Texture(const Texture & value) {
        mOpenGLTexture = OpenGLTextureFactory::initTexture(value.mPath);
        mPath = value.mPath;
      }

      explicit Texture(
          const char * path, bool flipX = false, bool flipY = false) :
          mOpenGLTexture(OpenGLTextureFactory::initTexture(path, flipX, flipY)),
          mPath(path) {}

      explicit Texture(QOpenGLTexture * texture) : mOpenGLTexture(texture) {}

      ~Texture() { mOpenGLTexture->destroy(); }

      /*************************************************************************
       * Accessors
       ************************************************************************/

      [[nodiscard]] inline QOpenGLTexture & getOpenGLTexture() const {
        return *mOpenGLTexture;
      }

      [[nodiscard]] inline std::string getPath() const { return mPath; }

      /*************************************************************************
       * Setters
       ************************************************************************/

      void setTexture(
          const std::string & path, bool flipX = false, bool flipY = false) {
        mOpenGLTexture =
            OpenGLTextureFactory::initTexture(path.data(), flipX, flipY);
        mPath = path.data();
      }

      void setTexture(
          const char * path, bool flipX = false, bool flipY = false) {
        mOpenGLTexture = OpenGLTextureFactory::initTexture(path, flipX, flipY);
        mPath = path;
      }

      virtual inline void setCubeMap(const char * path) {
        mOpenGLTexture = OpenGLTextureFactory::initCubeMap(path);
        mPath = path;
      }

      virtual inline void setCubeMap(
          const char * right, const char * top, const char * front,
          const char * left, const char * bottom, const char * back) {
        mOpenGLTexture = OpenGLTextureFactory::initCubeMap(
            right, top, front, left, bottom, back);
      }

      virtual inline void setCubeMap(
          const QImage & right, const QImage & top, const QImage & front,
          const QImage & left, const QImage & bottom, const QImage & back) {
        mOpenGLTexture = OpenGLTextureFactory::initCubeMap(
            right, top, front, left, bottom, back);
      }

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      [[nodiscard]] inline bool hasTexture() const {
        return mOpenGLTexture != Q_NULLPTR;
      }

    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/
      inline void setTexture(QOpenGLTexture * texture) {
        mOpenGLTexture = texture;
      }

      QOpenGLTexture * mOpenGLTexture = Q_NULLPTR;
      /* Path to this texture on disk or Qt resource. */
      const char * mPath {};
  };

}  // namespace Qtk

#endif  // QTOPENGL_TEXTURE_H
