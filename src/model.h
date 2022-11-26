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
  /**
   * 3D models will store this data for each vertex in geometry.
   */
  struct QTKAPI ModelVertex {
      QVector3D mPosition;
      QVector3D mNormal;
      QVector2D mTextureCoord;
      QVector3D mTangent;
      QVector3D mBitangent;
  };

  /**
   * Struct to store model textures. 3D Models may have multiple.
   */
  struct QTKAPI ModelTexture {
      GLuint mID {};
      QOpenGLTexture * mTexture {};
      std::string mType {};
      std::string mPath {};
  };

  class Model;

  /**
   * Mesh class specialized for storing 3D model data.
   * Eventually this can be consolidated into a more generic class.
   */
  class QTKAPI ModelMesh : protected QOpenGLFunctions {
    public:
      /*************************************************************************
       * Typedefs
       ************************************************************************/

      friend Model;
      typedef std::vector<ModelVertex> Vertices;
      typedef std::vector<GLuint> Indices;
      typedef std::vector<ModelTexture> Textures;

      /*************************************************************************
       * Constructors, Destructors
       ************************************************************************/

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

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      inline void draw() { draw(*mProgram); }

      void draw(QOpenGLShaderProgram & shader);

      /*************************************************************************
       * Public Members
       ************************************************************************/

      Vertices mVertices {};
      Indices mIndices {};
      Textures mTextures {};
      Transform3D mTransform;

    private:
      /*************************************************************************
       * Private Methods
       ************************************************************************/

      void initMesh(const char * vert, const char * frag);

      /*************************************************************************
       * Private Members
       ************************************************************************/

      QOpenGLBuffer *mVBO, *mEBO;
      QOpenGLVertexArrayObject * mVAO;
      QOpenGLShaderProgram * mProgram;
  };

  /**
   * Model object that has a ModelMesh.
   * Top-level object that represents 3D models stored within a scene.
   */
  class QTKAPI Model : public QObject {
      Q_OBJECT

    public:
      /*************************************************************************
       * Typedefs
       ************************************************************************/

      /* ModelManager typedef that will manage global model access. */
      typedef QHash<QString, Model *> ModelManager;

      /*************************************************************************
       * Constructors, Destructors
       ************************************************************************/

      // Default model shaders are provided but we can override them in the ctor
      inline Model(
          const char * name, const char * path,
          const char * vertexShader = ":/model-basic.vert",
          const char * fragmentShader = ":/model-basic.frag") :
          mName(name),
          mModelPath(path), mVertexShader(vertexShader),
          mFragmentShader(fragmentShader) {
        loadModel(path);
      }

      inline ~Model() override { mManager.remove(mName); }

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      void draw();
      void draw(QOpenGLShaderProgram & shader);

      /**
       * Flip a texture associated with this model
       *
       * @param fileName The name of the texture to flip as it is stored on disk
       * @param flipX Flip the texture along the X axis
       * @param flipY Flip the texture along the Y axis
       */
      void flipTexture(
          const std::string & fileName, bool flipX = false, bool flipY = true);

      /*************************************************************************
       * Setters
       ************************************************************************/

      /**
       * Sets a uniform value
       *
       * @tparam T The type of the value we are settings
       * @param location The uniform location
       * @param value The value to assign to the uniform
       */
      template <typename T> void setUniform(const char * location, T value) {
        for(auto & mesh : mMeshes) {
          mesh.mProgram->bind();
          mesh.mProgram->setUniformValue(location, value);
          mesh.mProgram->release();
        }
      }

      /*************************************************************************
       * Accessors
       ************************************************************************/

      /**
       * Accessor function for retrieving a ModelMesh globally.
       * The mesh is retrieved from the mManager private member.
       *
       * @param name The name of the model to load as it was constructed.
       * @return Pointer to the model stored within the scene.
       */
      static Model * getInstance(const char * name);

      Transform3D & getTransform() { return mTransform; }

    private:
      /*************************************************************************
       * Private Methods
       ************************************************************************/

      /**
       * Loads a model in .obj, .fbx, .gltf, and other formats.
       * For a full list of formats see assimp documentation:
       *  https://github.com/assimp/assimp/blob/master/doc/Fileformats.md
       *
       * Models should not be loaded into Qt resource system.
       * Instead pass an *absolute* path to this function.
       * Relative paths will break if Qtk is executed from different locations.
       *
       * Models can also be loaded from the `qtk/resource` directory using qrc
       * format loadModel(":/models/backpack/backpack.obj").
       * This does not use Qt resource system, it just provides similar syntax
       * for accessing files within the same `resources/` directory.
       *
       * See resourcemanager.h for more information on how this works.
       *
       * @param path Absolute path to a model in .obj or another format accepted
       * by assimp.
       */
      void loadModel(const std::string & path);

      void processNode(aiNode * node, const aiScene * scene);

      ModelMesh processMesh(aiMesh * mesh, const aiScene * scene);

      ModelMesh::Textures loadMaterialTextures(
          aiMaterial * mat, aiTextureType type, const std::string & typeName);

      void sortModels();

      /*************************************************************************
       * Private Members
       ************************************************************************/

      /* The position of this model in 3D space */
      Transform3D mTransform;

      /* Static QHash used to store and access models globally. */
      static ModelManager mManager;

      /* Container to store N loaded textures for this model. */
      ModelMesh::Textures mTexturesLoaded {};
      /* Container to store N loaded meshes for this model. */
      std::vector<ModelMesh> mMeshes {};
      /* The directory this model and it's textures are stored. */
      std::string mDirectory {};
      /* File names for shaders and 3D model on disk. */
      const char *mVertexShader, *mFragmentShader, *mModelPath;
      /* Name of the model object within the scene. */
      const char * mName;
  };
}  // namespace Qtk

#endif  // QTK_MODEL_H
