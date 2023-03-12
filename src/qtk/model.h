/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Model class for importing with Assimp                               ##
##        From following tutorials on learnopengl.com                         ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_MODEL_H
#define QTK_MODEL_H

// Qt
#include <QOpenGLFunctions>

// Assimp
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

// Qtk
#include "modelmesh.h"
#include "qtkapi.h"

namespace Qtk {
  /**
   * Model object that has a ModelMesh.
   * Top-level object that represents 3D models stored within a scene.
   */
  class QTKAPI Model : public Object {
    public:
      /*************************************************************************
       * Typedefs
       ************************************************************************/

      /** ModelManager typedef that will manage global model access. */
      typedef QHash<QString, Model *> ModelManager;

      /*************************************************************************
       * Constructors, Destructors
       ************************************************************************/

      /**
       * Constructs a Model
       * If no shaders are provided we will use default shaders.
       *
       * @param name Name to use for the Model's objectName.
       * @param path Path to the model to load for construction.
       * @param vertexShader Optional path to custom vertex shader.
       * @param fragmentShader Optional path to custom fragment shader.
       */
      inline Model(
          const char * name, const char * path,
          const char * vertexShader = ":/shaders/model-basic.vert",
          const char * fragmentShader = ":/shaders/model-basic.frag") :
          Object(name, QTK_MODEL),
          mModelPath(path), mVertexShader(vertexShader),
          mFragmentShader(fragmentShader) {
        loadModel(mModelPath);
      }

      inline ~Model() override { mManager.remove(getName().c_str()); }

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      /**
       * Draws the model using attached shader program.
       */
      void draw();

      /**
       * Draws the model using a custom shader program.
       *
       * @param shader Shader program to use to draw the model.
       */
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
       * Sets a uniform value for each ModelMesh within this Model.
       *
       * @tparam T The type of the value we are settings
       * @param location The uniform location
       * @param value The value to assign to the uniform
       */
      template <typename T>
      inline void setUniform(const char * location, T value) {
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
      [[nodiscard]] static Model * getInstance(const char * name);

      /**
       * @return Transform3D attached to this Model.
       */
      inline Transform3D & getTransform() { return mTransform; }

    private:
      /*************************************************************************
       * Private Methods
       ************************************************************************/

      /**
       * Loads a model in .obj, .fbx, .gltf, and other formats.
       * For a full list of formats see assimp documentation:
       *  https://github.com/assimp/assimp/blob/master/doc/Fileformats.md
       *
       * Large models should not be loaded into Qt resource system.
       * Instead pass an *absolute* path to this function.
       * Relative paths will break if Qtk is executed from different locations.
       *
       * @param path Absolute path to a model in .obj or another format accepted
       * by assimp.
       */
      void loadModel(const std::string & path);

      /**
       * Process a node in the model's geometry using Assimp.
       *
       * @param node The Assimp node to process.
       * @param scene The Assimp scene for the loaded model.
       */
      void processNode(aiNode * node, const aiScene * scene);

      /**
       * Process a mesh within a node using Assimp.
       *
       * @param mesh The Assimp mesh to process.
       * @param scene The Assimp scene for the loaded model.
       * @return
       */
      ModelMesh processMesh(aiMesh * mesh, const aiScene * scene);

      /**
       * Load a collection of material texture using Assimp.
       * This function loads diffuse, specular, and narmal material textures.
       * A Mesh may have many of any or all of the texture types above.
       * Models can have many Meshes attached.
       * This function returns all textures for a single Mesh within a Model.
       *
       * @param mat Loaded Assimp material.
       * @param type Type of the material.
       * @param typeName Texture type name in string format.
       * @return Collection of all textures for a single ModelMesh.
       */
      ModelMesh::Textures loadMaterialTextures(
          aiMaterial * mat, aiTextureType type, const std::string & typeName);

      /**
       * Sorts each mesh in the Model based on distance from the camera.
       * This is for efficient drawing in OpenGL by preventing the drawing of
       * objects not visible due to being partially or entirely behind another
       * object.
       */
      void sortModelMeshes();

      /*************************************************************************
       * Private Members
       ************************************************************************/

      /** Static QHash used to store and access models globally. */
      static ModelManager mManager;

      /** Container to store N loaded textures for this model. */
      ModelMesh::Textures mTexturesLoaded {};
      /** Container to store N loaded meshes for this model. */
      std::vector<ModelMesh> mMeshes {};
      /** The directory this model and it's textures are stored. */
      std::string mDirectory {};
      /** File names for shaders and 3D model on disk. */
      std::string mVertexShader, mFragmentShader, mModelPath;
  };
}  // namespace Qtk

#endif  // QTK_MODEL_H
