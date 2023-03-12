/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: ModelMesh class for importing 3D models with Assimp                 ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_MODELMESH_H
#define QTK_MODELMESH_H

#include <QOpenGLFunctions>

#include "object.h"
#include "transform3D.h"

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
      /** Texture ID for for this texture. */
      GLuint mID {};
      QOpenGLTexture * mTexture {};

      /**
       * Type of this texture in string format.
       * See calls to Model::loadMaterialTexture in Model::processMesh
       */
      std::string mType {};
      /** Path to the model on disk. */
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

      /**
       * Construct a ModelMesh.
       * If no shaders are provided defaults will be used.
       *
       * @param vertices Vertex data to use for this ModelMesh.
       * @param indices Index data to use for this ModelMesh.
       * @param textures Collection of ModelTextures for this ModelMesh.
       * @param vertexShader Path to vertex shader for this ModelMesh.
       * @param fragmentShader Path to fragment shader for this ModelMesh.
       */
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

      /**
       * Draw the model with the attached shader program.
       */
      inline void draw() { draw(*mProgram); }

      /**
       * Draw the model with a custom shader program.
       * @param shader The shader program to use for drawing the object.
       */
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

      /**
       * Initializes the buffers and shaders for this model mesh.
       *
       * @param vert Path to vertex shader to use for this model.
       * @param frag Path to fragment shader to use for this model.
       */
      void initMesh(const char * vert, const char * frag);

      /*************************************************************************
       * Private Members
       ************************************************************************/

      QOpenGLBuffer *mVBO, *mEBO;
      QOpenGLVertexArrayObject * mVAO;
      QOpenGLShaderProgram * mProgram;
  };
}  // namespace Qtk

#endif  // QTK_MODELMESH_H
