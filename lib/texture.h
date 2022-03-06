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


class Texture {
public:
  ~Texture() {}

  // QImage
  static QImage * initImage(const char * image,
                            bool flipX=false, bool flipY=false);

  // 2D Texture
  static QOpenGLTexture * initTexture2D(const char * texture,
                                        bool flipX=false, bool flipY=false);

  // Cube maps
  static QOpenGLTexture * initCubeMap(QImage right, QImage top,
                                      QImage front, QImage left,
                                      QImage bottom, QImage back);
  // Overloads for cube map initialization
  static QOpenGLTexture * initCubeMap(const char * tile);
  static QOpenGLTexture * initCubeMap(const char * right, const char * top,
                                      const char * front, const char * left,
                                      const char * bottom, const char * back);

private:
  // Private ctor to prevent creating instances of this class
  Texture() {}
};

#endif // QTOPENGL_TEXTURE_H
