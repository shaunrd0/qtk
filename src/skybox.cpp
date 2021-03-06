/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Skybox class using QtOpenGL                                         ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <scene.h>
#include <texture.h>

#include <skybox.h>


Skybox::Skybox(std::string right, std::string top, std::string front,
               std::string left, std::string bottom, std::string back,
               const std::string & name)
    : mCubeMap(Texture::initCubeMap(
    QImage(right.c_str()).mirrored(), QImage(top.c_str()),
    QImage(front.c_str()), QImage(left.c_str()),
    QImage(bottom.c_str()), QImage(back.c_str()))),
      mVBO(QOpenGLBuffer::VertexBuffer),
      mVertices(Cube(QTK_DRAW_ELEMENTS).vertices()),
      mIndices(Cube(QTK_DRAW_ELEMENTS).indices())
{ init();}

Skybox::Skybox(std::string name)
    : Skybox(":/right.png", ":/top.png", ":/front.png",
             ":/left.png", ":/bottom.png", ":/back.png",
             name)
{}

Skybox::Skybox(QOpenGLTexture * cubeMap, const std::string & name)
    : mCubeMap(cubeMap) { init();}


/*******************************************************************************
 * Public Member Functions
 ******************************************************************************/

void Skybox::draw()
{
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_FALSE);

  mVAO.bind();
  mProgram.bind();
  mCubeMap->bind();

  mProgram.setUniformValue("uProjectionMatrix", Scene::Projection());
  mProgram.setUniformValue("uViewMatrix", Scene::Camera().toMatrix());
  mProgram.setUniformValue("uTexture", 0);
  glDrawElements(GL_TRIANGLES, mIndices.size(),
                 GL_UNSIGNED_INT, mIndices.data());

  mCubeMap->release();
  mProgram.release();
  mVAO.release();

  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
  glActiveTexture(GL_TEXTURE0);
}


/*******************************************************************************
 * Private Member Functions
 ******************************************************************************/

void Skybox::init()
{
  // Set up shader program
  mProgram.create();
  mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox.vert");
  mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox.frag");
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
  mVBO.allocate(mVertices.data(),
                mVertices.size() * sizeof(mVertices[0]));

  // Enable attribute array for vertex positions
  mProgram.enableAttributeArray(0);
  mProgram.setAttributeBuffer(0, GL_FLOAT, 0,
                              3, sizeof(QVector3D));
  // Set shader texture unit to 0
  mProgram.setUniformValue("uTexture", 0);

  mVAO.release();
  mVBO.release();
  mProgram.release();
}
