/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: MeshRenderer class for quick object creation and drawing            ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QImageReader>

#include "meshrenderer.h"
#include "scene.h"
#include "texture.h"

using namespace Qtk;

// Static QHash that holds all MeshRenderer instances using their mName as keys
Qtk::MeshRenderer::MeshManager Qtk::MeshRenderer::sInstances;

/*******************************************************************************
 * Constructors / Destructors
 ******************************************************************************/

MeshRenderer::MeshRenderer(
    const char * name, Vertices vertices, Indices indices, DrawMode mode) :
    MeshRenderer(
        name, ShapeBase(mode, std::move(vertices), std::move(indices))) {}

MeshRenderer::MeshRenderer(const char * name) :
    MeshRenderer(name, Cube(QTK_DRAW_ELEMENTS)) {}

MeshRenderer::MeshRenderer(const char * name, const ShapeBase & shape) :
    Object(name, shape, QTK_MESH), mVertexShader(":/shaders/multi-color.vert"),
    mFragmentShader(":/shaders/multi-color.frag"), mDrawType(GL_TRIANGLES) {
  mShape = Shape(shape);
  init();
  sInstances.insert(name, this);
}

MeshRenderer::~MeshRenderer() {
  sInstances.remove(mName.c_str());
}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

void MeshRenderer::init() {
  if(mVAO.isCreated()) {
    mVAO.destroy();
  }
  if(mProgram.isLinked()) {
    mProgram.removeAllShaders();
  }
  if(mVBO.isCreated()) {
    mVBO.destroy();
  }

  mVAO.create();
  mVAO.bind();

  mProgram.create();
  mProgram.addShaderFromSourceFile(
      QOpenGLShader::Vertex, mVertexShader.c_str());
  mProgram.addShaderFromSourceFile(
      QOpenGLShader::Fragment, mFragmentShader.c_str());
  mProgram.link();
  mProgram.bind();

  mVBO.create();
  mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
  mVBO.bind();

  // Combine position and color data into one vector, allowing us to use one VBO
  Vertices combined;
  combined.reserve(getVertices().size() + getColors().size());
  combined.insert(combined.end(), getVertices().begin(), getVertices().end());
  combined.insert(combined.end(), getColors().begin(), getColors().end());

  mVBO.allocate(combined.data(), combined.size() * sizeof(combined[0]));

  // Enable position attribute
  mProgram.enableAttributeArray(0);
  mProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(QVector3D));
  // Enable color attribute, setting offset to total size of vertices()
  mProgram.enableAttributeArray(1);
  mProgram.setAttributeBuffer(
      1, GL_FLOAT, getVertices().size() * sizeof(getVertices()[0]), 3,
      sizeof(QVector3D));

  mVBO.release();

  mProgram.release();
  mVAO.release();
}

void MeshRenderer::draw() {
  bindShaders();
  mVAO.bind();

  if(mTexture.hasTexture()) {
    mTexture.getOpenGLTexture().bind();
  }

  // TODO: Automate uniforms some other way
  setUniformMVP();

  if(mShape.mDrawMode == QTK_DRAW_ARRAYS) {
    glDrawArrays(mDrawType, 0, getVertices().size());
  } else if(
      mShape.mDrawMode == QTK_DRAW_ELEMENTS
      || mShape.mDrawMode == QTK_DRAW_ELEMENTS_NORMALS) {
    glDrawElements(
        mDrawType, mShape.mIndices.size(), GL_UNSIGNED_INT,
        mShape.mIndices.data());
  }

  if(mTexture.hasTexture()) {
    mTexture.getOpenGLTexture().release();
  }

  mVAO.release();
  releaseShaders();
}

void MeshRenderer::enableAttributeArray(int location) {
  ShaderBindScope lock(&mProgram, mBound);
  mVAO.bind();
  mProgram.enableAttributeArray(location);
  mVAO.release();
}

void MeshRenderer::reallocateTexCoords(const TexCoords & t, unsigned dims) {
  mVAO.bind();
  mNBO.destroy();
  mNBO.create();
  mNBO.bind();
  mNBO.allocate(t.data(), t.size() * sizeof(t[0]));
  enableAttributeArray(1);
  if(dims == 2) {
    setAttributeBuffer(1, GL_FLOAT, 0, 2, sizeof(QVector2D));
  } else if(dims == 3) {
    setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));
  }
  mNBO.release();
  mVAO.release();
}

void MeshRenderer::reallocateNormals(const Normals & n, unsigned dims) {
  // TODO: Store state to track if buffer objects are bound
  mVAO.bind();
  mNBO.destroy();
  mNBO.create();
  mNBO.bind();
  mNBO.allocate(n.data(), n.size() * sizeof(n[0]));
  enableAttributeArray(1);
  if(dims == 2) {
    setAttributeBuffer(1, GL_FLOAT, 0, 2, sizeof(QVector2D));
  } else if(dims == 3) {
    setAttributeBuffer(1, GL_FLOAT, 0, 3, sizeof(QVector3D));
  }
  mNBO.release();
  mVAO.release();
}

void MeshRenderer::setShaders(
    const std::string & vert, const std::string & frag) {
  mVertexShader = vert;
  mFragmentShader = frag;
  init();
}

void MeshRenderer::setUniformMVP(
    const char * model, const char * view, const char * projection) {
  ShaderBindScope lock(&mProgram, mBound);
  mProgram.setUniformValue(projection, Scene::getProjectionMatrix());
  mProgram.setUniformValue(view, Scene::getViewMatrix());
  mProgram.setUniformValue(model, mTransform.toMatrix());
}

void MeshRenderer::setShape(const Shape & value) {
  Object::setShape(value);
  init();
}

void MeshRenderer::setColor(const QVector3D & color) {
  if(mShape.mColors.empty()) {
    for(const auto & vertex : mShape.getVertices()) {
      mShape.mColors.push_back(color);
    }
  } else {
    for(int i = 0; i < mShape.getColors().size(); i++) {
      mShape.mColors[i] = color;
    }
  }
  // TODO: Factor this out so we don't need to reinitialize
  init();
}

void MeshRenderer::setAttributeBuffer(
    int location, GLenum type, int offset, int tupleSize, int stride) {
  ShaderBindScope lock(&mProgram, mBound);
  mVAO.bind();
  mProgram.setAttributeBuffer(location, type, offset, tupleSize, stride);
  mVAO.release();
}

/*******************************************************************************
 * Static Public Methods
 ******************************************************************************/

// Static member function to retrieve instances of MeshRenderers
MeshRenderer * MeshRenderer::getInstance(const QString & name) {
  if(!sInstances.contains(name)) {
#if QTK_DEBUG
    qDebug() << "Attempt to access MeshRenderer instance that does not exist! ("
             << qPrintable(name) << ")\n";
#endif
    return nullptr;
  }
  return sInstances[name];
}
