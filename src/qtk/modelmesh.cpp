/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: ModelMesh class for importing 3D models with Assimp                 ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "modelmesh.h"
#include "scene.h"
#include "shaders.h"

using namespace Qtk;

/*******************************************************************************
 * Public Member Functions
 ******************************************************************************/

void ModelMesh::draw(QOpenGLShaderProgram & shader)
{
  mVAO->bind();
  // Bind shader
  shader.bind();

  // Set Model View Projection values
  shader.setUniformValue("uModel", mTransform.toMatrix());
  shader.setUniformValue("uView", Scene::getViewMatrix());
  shader.setUniformValue("uProjection", Scene::getProjectionMatrix());

  GLuint diffuseCount = 1;
  GLuint specularCount = 1;
  GLuint normalCount = 1;
  for (GLuint i = 0; i < mTextures.size(); i++) {
    // Activate the current texture index by adding offset to GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0 + i);
    mTextures[i].mTexture->bind();

    // Get a name for the texture using a known convention -
    // Diffuse:   material.texture_diffuse1, material.texture_diffuse2, ...
    // Specular:   material.texture_specular1, material.texture_specular2, ...
    std::string number;
    std::string name = mTextures[i].mType;
    if (name == "texture_diffuse") {
      number = std::to_string(diffuseCount++);
    }
    if (name == "texture_specular") {
      number = std::to_string(specularCount++);
    }
    if (name == "texture_normal") {
      number = std::to_string(normalCount++);
    }

    // Set the uniform to track this texture ID using our naming convention
    shader.setUniformValue((name + number).c_str(), i);
  }

  // Always reset active texture to GL_TEXTURE0 before we draw.
  // This is important for models with no textures.
  glActiveTexture(GL_TEXTURE0);

  // Draw the mesh
  glDrawElements(
      GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, mIndices.data());

  // Release shader, textures
  for (const auto & texture : mTextures) {
    texture.mTexture->release();
  }
  shader.release();
  mVAO->release();
}

/*******************************************************************************
 * Private Member Functions
 ******************************************************************************/

void ModelMesh::initMesh(const std::string & vert, const std::string & frag)
{
  initializeOpenGLFunctions();

  // Create VAO, VBO, EBO
  mVAO->create();
  mVBO->create();
  mEBO->create();

  mVAO->bind();

  // Allocate VBO
  mVBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
  mVBO->bind();

  mVBO->allocate(mVertices.data(), mVertices.size() * sizeof(mVertices[0]));

  // Allocate EBO
  mEBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
  mEBO->bind();
  mEBO->allocate(mIndices.data(), mIndices.size() * sizeof(mIndices[0]));
  mEBO->release();

  // Load and link shaders
  if (!vert.empty()) {
    mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vert.c_str());
  } else {
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                      QTK_SHADER_VERTEX_MODEL);
  }

  if (!frag.empty()) {
    mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, frag.c_str());
  } else {
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                      QTK_SHADER_FRAGMENT_MODEL);
  }

  if (!mProgram->link()) {
    qDebug() << "Failed to link shader: " << mProgram->log();
  }
  if (!mProgram->bind()) {
    qDebug() << "Failed to bind shader: " << mProgram->log();
  }

  // Positions
  mProgram->enableAttributeArray(0);
  mProgram->setAttributeBuffer(
      0, GL_FLOAT, offsetof(ModelVertex, mPosition), 3, sizeof(ModelVertex));

  // Normals
  mProgram->enableAttributeArray(1);
  mProgram->setAttributeBuffer(
      1, GL_FLOAT, offsetof(ModelVertex, mNormal), 3, sizeof(ModelVertex));

  // Texture Coordinates
  mProgram->enableAttributeArray(2);
  mProgram->setAttributeBuffer(2,
                               GL_FLOAT,
                               offsetof(ModelVertex, mTextureCoord),
                               2,
                               sizeof(ModelVertex));

  // Vertex tangents
  mProgram->enableAttributeArray(3);
  mProgram->setAttributeBuffer(
      3, GL_FLOAT, offsetof(ModelVertex, mTangent), 3, sizeof(ModelVertex));

  // Vertex bitangents
  mProgram->enableAttributeArray(4);
  mProgram->setAttributeBuffer(
      4, GL_FLOAT, offsetof(ModelVertex, mBitangent), 3, sizeof(ModelVertex));

  mProgram->release();
  mVBO->release();
  mVAO->release();
}
