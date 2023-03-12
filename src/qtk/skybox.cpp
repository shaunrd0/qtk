/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Skybox class using QtOpenGL                                         ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "skybox.h"
#include "scene.h"
#include "texture.h"

using namespace Qtk;

/*******************************************************************************
 * Constructors / Destructors
 ******************************************************************************/

Skybox::Skybox(const std::string & name) :
    Skybox(
        ":/textures/skybox/right.png", ":/textures/skybox/top.png",
        ":/textures/skybox/front.png", ":/textures/skybox/left.png",
        ":/textures/skybox/bottom.png", ":/textures/skybox/back.png", name) {}

Skybox::Skybox(QOpenGLTexture * cubeMap, const std::string & name) {
  mTexture.setTexture(cubeMap);
  init();
}

Skybox::Skybox(
    const std::string & right, const std::string & top,
    const std::string & front, const std::string & left,
    const std::string & bottom, const std::string & back,
    const std::string & name) :
    mVBO(QOpenGLBuffer::VertexBuffer),
    mVertices(Cube(QTK_DRAW_ELEMENTS).getVertices()),
    mIndices(Cube(QTK_DRAW_ELEMENTS).getIndexData()) {
  init();
  mTexture.setCubeMap(
      QImage(right.c_str()).mirrored(), QImage(top.c_str()),
      QImage(front.c_str()), QImage(left.c_str()), QImage(bottom.c_str()),
      QImage(back.c_str()));
}

/*******************************************************************************
 * Public Member Functions
 ******************************************************************************/

void Skybox::draw() {
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_FALSE);

  mVAO.bind();
  mProgram.bind();
  mTexture.getOpenGLTexture().bind();

  mProgram.setUniformValue("uProjectionMatrix", Scene::getProjectionMatrix());
  mProgram.setUniformValue("uViewMatrix", Scene::getCamera().toMatrix());
  mProgram.setUniformValue("uTexture", 0);
  glDrawElements(
      GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, mIndices.data());

  mTexture.getOpenGLTexture().bind();
  mProgram.release();
  mVAO.release();

  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glActiveTexture(GL_TEXTURE0);
}

/*******************************************************************************
 * Private Member Functions
 ******************************************************************************/

void Skybox::init() {
  initializeOpenGLFunctions();

  // Set up shader program
  mProgram.create();
  mProgram.addShaderFromSourceFile(
      QOpenGLShader::Vertex, ":/shaders/skybox.vert");
  mProgram.addShaderFromSourceFile(
      QOpenGLShader::Fragment, ":/shaders/skybox.frag");
  mProgram.link();
  mProgram.bind();

  // Setup VAO
  mVAO.create();
  mVAO.bind();

  // Setup VBO for vertex position data
  mVBO.create();
  mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
  mVBO.bind();
  // Allocate vertex positions into VBO
  mVBO.allocate(mVertices.data(), mVertices.size() * sizeof(mVertices[0]));

  // Enable attribute array for vertex positions
  mProgram.enableAttributeArray(0);
  mProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));
  // Set shader texture unit to 0
  mProgram.setUniformValue("uTexture", 0);

  mVAO.release();
  mVBO.release();
  mProgram.release();
}
