/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Model classes for importing with Assimp                             ##
##        From following tutorials on learnopengl.com                         ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_MODEL_H
#define QTK_MODEL_H

// QT
#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

// Assimp
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

// QTK
#include <object.h>
#include <qtkapi.h>
#include <transform3D.h>

namespace Qtk {
  struct QTKAPI ModelVertex {
      QVector3D mPosition;
      QVector3D mNormal;
      QVector3D mTangent;
      QVector3D mBitangent;
      QVector2D mTextureCoord;
  };

  struct QTKAPI ModelTexture {
      GLuint mID {};
      QOpenGLTexture * mTexture {};
      std::string mType {};
      std::string mPath {};
  };

  class Model;

  class QTKAPI ModelMesh : protected QOpenGLFunctions {
    public:
      friend Model;
      typedef std::vector<ModelVertex> Vertices;
      typedef std::vector<GLuint> Indices;
      typedef std::vector<ModelTexture> Textures;

      // Constructors, Destructors
      ModelMesh(
          Vertices vertices, Indices indices, Textures textures,
          const char * vertexShader = ":/model-basic.vert",
          const char * fragmentShader = ":/model-basic.frag") :
          mProgram(new QOpenGLShaderProgram),
          mVAO(new QOpenGLVertexArrayObject),
          mVBO(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer)),
          mEBO(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer)),
          mVertices(std::move(vertices)), mIndices(std::move(indices)),
          mTextures(std::move(textures)) {
        initMesh(vertexShader, fragmentShader);
      }

      ~ModelMesh() = default;

    private:
      void initMesh(const char * vert, const char * frag);

      // ModelMesh Private Members
      QOpenGLBuffer *mVBO, *mEBO;
      QOpenGLVertexArrayObject * mVAO;
      QOpenGLShaderProgram * mProgram;

    public:
      inline void draw() { draw(*mProgram); }

      void draw(QOpenGLShaderProgram & shader);

      // ModelMesh Public Members
      Vertices mVertices {};
      Indices mIndices {};
      Textures mTextures {};
      Transform3D mTransform;
  };

  class QTKAPI Model : public QObject {
      Q_OBJECT

    public:
      inline Model(
          const char * name, const char * path,
          const char * vertexShader = ":/model-basic.vert",
          const char * fragmentShader = ":/model-basic.frag") :
          mName(name),
          mVertexShader(vertexShader), mFragmentShader(fragmentShader) {
        loadModel(path);
      }

      inline ~Model() override { mManager.remove(mName); }

      void draw();
      void draw(QOpenGLShaderProgram & shader);

      void flipTexture(
          const std::string & fileName, bool flipX = false, bool flipY = true);

      template <typename T> void setUniform(const char * location, T value) {
        for(auto & mesh : mMeshes) {
          mesh.mProgram->bind();

          mesh.mProgram->setUniformValue(location, value);

          mesh.mProgram->release();
        }
      }

      Transform3D mTransform;

      static Model * getInstance(const char * name);

      typedef QHash<QString, Model *> ModelManager;

    private:
      static ModelManager mManager;
      void loadModel(const std::string & path);
      void processNode(aiNode * node, const aiScene * scene);
      ModelMesh processMesh(aiMesh * mesh, const aiScene * scene);
      ModelMesh::Textures loadMaterialTextures(
          aiMaterial * mat, aiTextureType type, const std::string & typeName);
      void sortModels();

      // Model Private Members

      ModelMesh::Textures mTexturesLoaded {};
      std::vector<ModelMesh> mMeshes {};
      std::string mDirectory {};
      const char *mVertexShader, *mFragmentShader, *mName;
  };
}  // namespace Qtk

#endif  // QTK_MODEL_H
