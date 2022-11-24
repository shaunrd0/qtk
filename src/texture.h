/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Texture class to help with texture and image initializations        ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTOPENGL_TEXTURE_H
#define QTOPENGL_TEXTURE_H

#include <QOpenGLTexture>
#include <utility>

#include <qtkapi.h>

namespace Qtk {
  class QTKAPI OpenGLTextureFactory {
    public:
      ~OpenGLTextureFactory() = default;

      // QImage
      static QImage * initImage(
          const char * image, bool flipX = false, bool flipY = false);

      // 2D Texture
      static QOpenGLTexture * initTexture2D(
          const char * texture, bool flipX = false, bool flipY = false);

      // Cube maps
      static QOpenGLTexture * initCubeMap(
          const QImage & right, const QImage & top, const QImage & front,
          const QImage & left, const QImage & bottom, const QImage & back);
      // Overloads for cube map initialization
      static QOpenGLTexture * initCubeMap(const char * tile);
      static QOpenGLTexture * initCubeMap(
          const char * right, const char * top, const char * front,
          const char * left, const char * bottom, const char * back);

    private:
      // Private ctor to prevent creating instances of this class
      OpenGLTextureFactory() = default;
  };

  // TODO: Struct for (re)storing texture state
  class Texture {
    public:
      Texture() = default;
      Texture(const Texture & value) {
        mOpenGLTexture = OpenGLTextureFactory::initTexture2D(value.mPath);
        mPath = value.mPath;
      }
      explicit Texture(
          const char * path, bool flipX = false, bool flipY = false) :
          mOpenGLTexture(
              OpenGLTextureFactory::initTexture2D(path, flipX, flipY)),
          mPath(path) {}
      explicit Texture(QOpenGLTexture * texture) : mOpenGLTexture(texture) {}

      ~Texture() { mOpenGLTexture->destroy(); }

      [[nodiscard]] inline QOpenGLTexture & getOpenGLTexture() const {
        return *mOpenGLTexture;
      }

      [[nodiscard]] inline std::string getPath() const { return mPath; }

      void setTexture(
          const std::string & path, bool flipX = false, bool flipY = false) {
        mOpenGLTexture =
            OpenGLTextureFactory::initTexture2D(path.data(), flipX, flipY);
        mPath = path.data();
      }

      void setTexture(
          const char * path, bool flipX = false, bool flipY = false) {
        mOpenGLTexture =
            OpenGLTextureFactory::initTexture2D(path, flipX, flipY);
        mPath = path;
      }

      // TODO: This is unsafe because we don't have a path. Encapsulate it.
      inline void setTexture(QOpenGLTexture * texture) {
        mOpenGLTexture = texture;
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

      [[nodiscard]] inline bool hasTexture() const {
        return mOpenGLTexture != Q_NULLPTR;
      }

    private:
      QOpenGLTexture * mOpenGLTexture = Q_NULLPTR;
      const char * mPath {};
  };

}  // namespace Qtk

#endif  // QTOPENGL_TEXTURE_H
