/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Texture class to help with texture and image initializations        ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QDebug>
#include <QImageReader>

#include "texture.h"

using namespace Qtk;

QImage * OpenGLTextureFactory::initImage(
    const char * image, bool flipX, bool flipY) {
  // Qt6 limits loaded images to 256MB by default
  QImageReader::setAllocationLimit(512);
  auto loadedImage = new QImage(QImage(image).mirrored(flipX, flipY));
  if(loadedImage->isNull()) {
    qDebug() << "[Qtk::OpenGLTextureFactory] Error loading image: " << image
             << "\nSupported types: " << QImageReader::supportedImageFormats();
    return Q_NULLPTR;
  }

  return loadedImage;
}

QOpenGLTexture * OpenGLTextureFactory::initTexture(
    const char * texture, bool flipX, bool flipY) {
  QImage * image = initImage(texture, flipX, flipY);
  auto newTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
  newTexture->setData(*image);
  newTexture->setWrapMode(QOpenGLTexture::Repeat);
  newTexture->setMinMagFilters(
      QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
  delete image;
  return newTexture;
}

QOpenGLTexture * OpenGLTextureFactory::initCubeMap(const char * tile) {
  return initCubeMap(
      QImage(tile), QImage(tile), QImage(tile), QImage(tile), QImage(tile),
      QImage(tile));
}

QOpenGLTexture * OpenGLTextureFactory::initCubeMap(
    const char * right, const char * top, const char * front, const char * left,
    const char * bottom, const char * back) {
  return initCubeMap(
      QImage(right), QImage(top), QImage(front), QImage(left), QImage(bottom),
      QImage(back));
}

QOpenGLTexture * OpenGLTextureFactory::initCubeMap(
    const QImage & right, const QImage & top, const QImage & front,
    const QImage & left, const QImage & bottom, const QImage & back) {
  auto texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
  std::vector<QImage> faceTextures = {right, top, front, left, bottom, back};
  // Initialize skybox cubemap texture
  texture->create();
  texture->bind();
  // For each cube map face
  std::vector<QOpenGLTexture::CubeMapFace> faces = {
      QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::CubeMapPositiveY,
      QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::CubeMapNegativeX,
      QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::CubeMapNegativeZ};
  int i = 0;
  for(const auto & face : faces) {
    QImage faceImage(faceTextures[i]);
    if(faceImage.isNull()) {
      qDebug() << "Error loading cube map image\n";
    }
    faceImage = faceImage.convertToFormat(QImage::Format_RGBA8888);

    // On the first iteration, set format and allocate texture storage
    if(face == QOpenGLTexture::CubeMapPositiveX) {
      // This also needs to happen on the first iteration, anyways
      texture->setSize(
          faceImage.width(), faceImage.height(), faceImage.depth());
      texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
      texture->allocateStorage();
    }

    texture->setData(
        0, 0, face, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
        faceImage.constBits());
    i++;
  }

  texture->setWrapMode(QOpenGLTexture::ClampToEdge);
  texture->generateMipMaps();
  texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  texture->setMagnificationFilter(QOpenGLTexture::Linear);
  texture->release();
  return texture;
}
