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
#include <qtkapi.h>

namespace Qtk {
  class QTKAPI Object : public QObject {
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

    inline const Vertices & vertices() { return mShape.mVertices;}
    inline const Indices & indices() { return mShape.mIndices;}
    inline const Colors & colors() { return mShape.mColors;}
    inline const TexCoords & texCoords() { return mShape.mTexCoords;}
    inline const Normals & normals() { return mShape.mNormals;}
    inline QOpenGLTexture & texture() const { return *mTexture;}

    virtual inline void setVertices(const Vertices & value) { mShape.mVertices = value;}
    virtual inline void setIndices(const Indices & value) { mShape.mIndices = value;}
    virtual inline void setColors(const Colors & value) { mShape.mColors = value;}
    virtual inline void setTexCoords(const TexCoords & value) { mShape.mTexCoords = value;}
    virtual inline void setNormals(const Normals & value) { mShape.mNormals = value;}
    virtual inline void setShape(const Shape & value) { mShape = value;}

    QOpenGLBuffer mVBO, mNBO;
    QOpenGLVertexArrayObject mVAO;
    QOpenGLShaderProgram mProgram;

    Transform3D mTransform;
    Shape mShape;

    const char * mName;
  private:
    QOpenGLTexture * mTexture;
  };
}

#endif // QTK_OBJECT_H
