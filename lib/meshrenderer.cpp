/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: MeshRenderer class for quick object creation and drawing            ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QImageReader>

#include <scene.h>
#include <texture.h>

#include <meshrenderer.h>


// Static QHash that holds all MeshRenderer instances using their mName as keys
MeshRenderer::MeshManager MeshRenderer::sInstances;

MeshRenderer::MeshRenderer(const char * name, const ShapeBase & shape)
    : Object(name, shape), mVertexShader(":/multi-color.vert"),
      mFragmentShader(":/multi-color.frag"), mDrawType(GL_TRIANGLES),
      mHasTexture(false)
{
  mShape = Shape(shape);
  init();
  sInstances.insert(name, this);
}

MeshRenderer::~MeshRenderer()
{
  if (mHasTexture) {
    mTexture->destroy();
  }
  sInstances.remove(mName);
}


// Static member function to retrieve instances of MeshRenderers
MeshRenderer * MeshRenderer::getInstance(const QString & name)
{ return sInstances[name];}


/*******************************************************************************
 * Public Member Functions
 ******************************************************************************/

void MeshRenderer::init()
{
  if (mVAO.isCreated()) mVAO.destroy();
  if (mProgram.isLinked()) mProgram.removeAllShaders();
  if (mVBO.isCreated()) mVBO.destroy();

  mVAO.create();
  mVAO.bind();

  mProgram.create();
  mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                   mVertexShader.c_str());
  mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                   mFragmentShader.c_str());
  mProgram.link();
  mProgram.bind();

  mVBO.create();
  mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
  mVBO.bind();

  // Combine position and color data into one vector, allowing us to use one VBO
  Vertices combined;
  combined.reserve(vertices().size() + colors().size());
  combined.insert(combined.end(), vertices().begin(), vertices().end());
  combined.insert(combined.end(), colors().begin(), colors().end());

  mVBO.allocate(combined.data(),
                combined.size() * sizeof(combined[0]));

  // Enable position attribute
  mProgram.enableAttributeArray(0);
  mProgram.setAttributeBuffer(0, GL_FLOAT, 0,
                              3, sizeof(QVector3D));
  // Enable color attribute, setting offset to total size of vertices()
  mProgram.enableAttributeArray(1);
  mProgram.setAttributeBuffer(1, GL_FLOAT,
                              vertices().size() * sizeof(vertices()[0]),
                              3, sizeof(QVector3D));

  mVBO.release();

  mProgram.release();
  mVAO.release();
}

void MeshRenderer::draw()
{
  mProgram.bind();
  mVAO.bind();

  if(mHasTexture) {
    mTexture->bind();
  }

  // TODO: Automate uniforms some other way
  setUniformMVP();

  if (mShape.mDrawMode == QTK_DRAW_ARRAYS) {
    glDrawArrays(mDrawType, 0, vertices().size());
  }
  else if (mShape.mDrawMode == QTK_DRAW_ELEMENTS
           || mShape.mDrawMode == QTK_DRAW_ELEMENTS_NORMALS) {
    glDrawElements(mDrawType, mShape.mIndices.size(),
                   GL_UNSIGNED_INT, mShape.mIndices.data());
  }

  if(mHasTexture) {
    mTexture->release();
  }

  mVAO.release();
  mProgram.release();
}

void MeshRenderer::setShaders(const std::string & vert, const std::string & frag)
{
  mVertexShader = vert;
  mFragmentShader = frag;
}

void MeshRenderer::setUniformMVP(const char * model, const char * view,
                                 const char * projection)
{
  mProgram.setUniformValue(projection, Scene::Projection());
  mProgram.setUniformValue(view, Scene::View());
  mProgram.setUniformValue(model, mTransform.toMatrix());
}

void MeshRenderer::setColor(const QVector3D & color)
{
  if (mShape.mColors.empty()) {
    for (const auto & vertex : mShape.vertices()) {
      mShape.mColors.push_back(color);
    }
  }
  else {
    for (int i = 0; i < mShape.colors().size(); i++) {
      mShape.mColors[i] = color;
    }
  }
}

void MeshRenderer::setTexture(const char * path)
{
  mTexture = new QOpenGLTexture(*Texture::initImage(path));
  mHasTexture = true;
}

void MeshRenderer::setTexture(QOpenGLTexture * texture)
{
  mTexture = texture;
  mHasTexture = true;
}


/*******************************************************************************
 * Inherited Virtual Member Functions
 ******************************************************************************/

void MeshRenderer::setShape(const Shape & value)
{
  Object::setShape(value);
  init();
}
