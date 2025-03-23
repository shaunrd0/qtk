/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
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

#include "qtkapi.h"
#include "shape.h"
#include "texture.h"

namespace Qtk
{
  class Model;

  /**
   * Object base class for objects that can exist within a scene.
   * An object could be a Cube, Skybox, 3D Model, or other standalone entities.
   */
  class QTKAPI Object : public QObject
  {
      Q_OBJECT

    public:
      /*************************************************************************
       * Typedefs
       ************************************************************************/

      friend MeshRenderer;
      friend Model;

      /**
       * Enum flag to identify Object type without casting.
       */
      enum Type { QTK_OBJECT, QTK_MESH, QTK_MODEL };

      /*************************************************************************
       * Constructors / Destructors
       ************************************************************************/

      // Initialize an object with no shape data assigned
      explicit Object(const char * name, Type type) :
          mName(name), mVBO(QOpenGLBuffer::VertexBuffer), mBound(false),
          mType(type)
      {
        setObjectName(name);
      }

      // Initialize an object with shape data assigned
      Object(const char * name, const ShapeBase & shape, Type type) :
          mName(name), mVBO(QOpenGLBuffer::VertexBuffer), mShape(shape),
          mBound(false), mType(type)
      {
        setObjectName(name);
      }

      ~Object() override = default;

      /*************************************************************************
       * Accessors
       ************************************************************************/

      [[nodiscard]] inline const Colors & getColors() const
      {
        return mShape.mColors;
      }

      [[nodiscard]] inline const Indices & getIndexData() const
      {
        return mShape.mIndices;
      }

      [[nodiscard]] inline const Normals & getNormals() const
      {
        return mShape.mNormals;
      }

      [[nodiscard]] inline const Shape & getShape() const { return mShape; }

      [[nodiscard]] inline const TexCoords & getTexCoords() const
      {
        return mShape.mTexCoords;
      }

      [[nodiscard]] inline const Texture & getTexture() const
      {
        return mTexture;
      }

      [[nodiscard]] inline const Vertices & getVertices() const
      {
        return mShape.mVertices;
      }

      [[nodiscard]] inline QString getName() const { return mName; }

      [[nodiscard]] inline const Type & getType() const { return mType; }

      [[nodiscard]] inline virtual const Transform3D & getTransform() const
      {
        return mTransform;
      }

      [[nodiscard]] inline virtual Transform3D & getTransform()
      {
        return mTransform;
      }

      [[nodiscard]] inline virtual std::string getVertexShader() const
      {
        return "Base Object has no vertex shader.";
      }

      [[nodiscard]] virtual inline std::string getFragmentShader() const
      {
        return "Base Object has no fragment shader.";
      }

      [[nodiscard]] virtual std::string getShaderSourceCode(
          QOpenGLShader::ShaderType shader_type) const;


      [[nodiscard]] virtual inline std::string getVertexShaderSourceCode() const
      {
        return getShaderSourceCode(QOpenGLShader::Vertex);
      }

      [[nodiscard]] virtual inline std::string getFragmentShaderSourceCode()
          const
      {
        return getShaderSourceCode(QOpenGLShader::Fragment);
      }

      /*************************************************************************
       * Setters
       ************************************************************************/

      virtual inline void setName(const QString & name) { mName = name; }

      virtual inline void setColors(const Colors & value)
      {
        mShape.mColors = value;
      }

      virtual inline void setIndices(const Indices & value)
      {
        mShape.mIndices = value;
      }

      virtual inline void setNormals(const Normals & value)
      {
        mShape.mNormals = value;
      }

      virtual inline void setShape(const Shape & value) { mShape = value; }

      virtual inline void setTexCoords(const TexCoords & value)
      {
        mShape.mTexCoords = value;
      }

      virtual inline void setTexture(const char * path,
                                     bool flipX = false,
                                     bool flipY = false)
      {
        mTexture.setTexture(path, flipX, flipY);
      }

      virtual inline void setCubeMap(const char * path)
      {
        mTexture.setCubeMap(path);
      }

      virtual inline void setTexture(const Texture & t)
      {
        mTexture.setTexture(t.getPath());
      }

      virtual inline void setVertices(const Vertices & value)
      {
        mShape.mVertices = value;
      }

      inline void setScaleX(double x)
      {
        mTransform.setScale(
            x, mTransform.getScale().y(), mTransform.getScale().z());
      }

      inline void setScaleY(double y)
      {
        mTransform.setScale(
            mTransform.getScale().x(), y, mTransform.getScale().z());
      }

      inline void setScaleZ(double z)
      {
        mTransform.setScale(
            mTransform.getScale().x(), mTransform.getScale().y(), z);
      }

      inline void setTranslationX(double x)
      {
        mTransform.setTranslation(x,
                                  mTransform.getTranslation().y(),
                                  mTransform.getTranslation().z());
      }

      inline void setTranslationY(double y)
      {
        mTransform.setTranslation(mTransform.getTranslation().x(),
                                  y,
                                  mTransform.getTranslation().z());
      }

      inline void setTranslationZ(double z)
      {
        mTransform.setTranslation(mTransform.getTranslation().x(),
                                  mTransform.getTranslation().y(),
                                  z);
      }

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      virtual inline void bindShaders()
      {
        mBound = true;
        mProgram.bind();
      }

      virtual inline void releaseShaders()
      {
        mBound = false;
        mProgram.release();
      }

      /*************************************************************************
       * Public Static Methods
       ************************************************************************/

      /**
       * Helper to disconnect a QObject connection, only if it's valid.
       * If the connection is valid and we fail to disconnect log a message.
       *
       * @param con QObject connection handle to disconnect.
       */
      static void disconnect(const QMetaObject::Connection & con)
      {
        if (con && !QObject::disconnect(con)) {
          qDebug() << "[Qtk] Failed to disconnect valid connection: " << con;
        }
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
      QString mName;
      bool mBound;
      Type mType = QTK_OBJECT;
  };
}  // namespace Qtk

#endif  // QTK_OBJECT_H
