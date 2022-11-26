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
#include <texture.h>

namespace Qtk {
  /**
   * Object base class for objects that can exist within a scene.
   * An object could be a Cube, Skybox, 3D Model, or other standalone entities.
   */
  class QTKAPI Object : public QObject {
      Q_OBJECT

    public:
      /*************************************************************************
       * Typedefs
       ************************************************************************/

      friend MeshRenderer;

      /*************************************************************************
       * Constructors / Destructors
       ************************************************************************/

      // Initialize an object with no shape data assigned
      explicit Object(const char * name) :
          mName(name), mVBO(QOpenGLBuffer::VertexBuffer), mBound(false) {}

      // Initialize an object with shape data assigned
      Object(const char * name, const ShapeBase & shape) :
          mName(name), mVBO(QOpenGLBuffer::VertexBuffer), mShape(shape),
          mBound(false) {}

      ~Object() override = default;

      /*************************************************************************
       * Accessors
       ************************************************************************/

      [[nodiscard]] inline const Colors & getColors() const {
        return mShape.mColors;
      }

      [[nodiscard]] inline const Indices & getIndexData() const {
        return mShape.mIndices;
      }

      [[nodiscard]] inline const Normals & getNormals() const {
        return mShape.mNormals;
      }

      [[nodiscard]] inline const Shape & getShape() const { return mShape; }

      [[nodiscard]] inline const TexCoords & getTexCoords() const {
        return mShape.mTexCoords;
      }

      [[nodiscard]] inline const Texture & getTexture() const {
        return mTexture;
      }

      [[nodiscard]] inline const Vertices & getVertices() const {
        return mShape.mVertices;
      }

      /*************************************************************************
       * Setters
       ************************************************************************/

      virtual inline void setColors(const Colors & value) {
        mShape.mColors = value;
      }

      virtual inline void setIndices(const Indices & value) {
        mShape.mIndices = value;
      }

      virtual inline void setNormals(const Normals & value) {
        mShape.mNormals = value;
      }

      virtual inline void setShape(const Shape & value) { mShape = value; }

      virtual inline void setTexCoords(const TexCoords & value) {
        mShape.mTexCoords = value;
      }

      virtual inline void setTexture(
          const char * path, bool flipX = false, bool flipY = false) {
        mTexture.setTexture(path, flipX, flipY);
      }

      virtual inline void setCubeMap(const char * path) {
        mTexture.setCubeMap(path);
      }

      virtual inline void setTexture(const Texture & t) {
        mTexture.setTexture(t.getPath());
      }

      virtual inline void setVertices(const Vertices & value) {
        mShape.mVertices = value;
      }

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      virtual inline void bindShaders() {
        mBound = true;
        mProgram.bind();
      }

      virtual inline void releaseShaders() {
        mBound = false;
        mProgram.release();
      }

    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/

      QOpenGLShaderProgram mProgram;
      QOpenGLBuffer mVBO, mNBO;
      QOpenGLVertexArrayObject mVAO;
      Transform3D mTransform;
      Shape mShape;
      Texture mTexture;
      const char * mName;
      bool mBound;
  };
}  // namespace Qtk

#endif  // QTK_OBJECT_H
