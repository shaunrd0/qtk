/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Object class for storing object data                                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_OBJECT_H
#define QTK_OBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

#include <mesh.h>
#include <texture.h>


class Object : public QObject {
Q_OBJECT

public:
  friend MeshRenderer;
  // Initialize an object with no shape data assigned
  Object(const char * name)
      : mName(name), mVBO(QOpenGLBuffer::VertexBuffer)
  { }
  // Initialize an object with shape data assigned
  Object(const char * name, const ShapeBase & shape)
      : mName(name), mVBO(QOpenGLBuffer::VertexBuffer), mShape(shape)
  { }

  ~Object() {}

  inline const Colors & getColors() { return mShape.mColors;}
  inline const Indices & getIndexData() { return mShape.mIndices;}
  inline const Normals & getNormals() { return mShape.mNormals;}
  inline const Shape & getShape() { return mShape;}
  inline const TexCoords & getTexCoords() { return mShape.mTexCoords;}
  inline Texture & getTexture() { return mTexture;}
  inline const Vertices & getVertices() { return mShape.mVertices;}

  virtual inline void setColors(const Colors & value) { mShape.mColors = value;}
  virtual inline void setIndices(const Indices & value) { mShape.mIndices = value;}
  virtual inline void setNormals(const Normals & value) { mShape.mNormals = value;}
  virtual inline void setShape(const Shape & value) { mShape = value;}
  virtual inline void setTexCoords(const TexCoords & value) { mShape.mTexCoords = value;}
  virtual inline void setTexture(const char * path, bool flipX=false, bool flipY=false)
  { mTexture.setTexture(OpenGLTextureFactory::initTexture2D(path, flipX, flipY));}
  virtual inline void setTexture(QOpenGLTexture * value) { mTexture.setTexture(value);}
  virtual inline void setVertices(const Vertices & value) { mShape.mVertices = value;}

  QOpenGLBuffer mVBO, mNBO;
  QOpenGLVertexArrayObject mVAO;
  QOpenGLShaderProgram mProgram;

  Transform3D mTransform;
  Shape mShape;
  Texture mTexture;

  const char * mName;
};

#endif // QTK_OBJECT_H
