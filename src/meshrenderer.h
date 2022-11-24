/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: MeshRenderer class for quick object creation and drawing            ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_MESHRENDERER_H
#define QTK_MESHRENDERER_H

#include <mesh.h>
#include <object.h>
#include <qtkapi.h>

#include <utility>

namespace Qtk {
  class QTKAPI ShaderBindScope {
    public:
      explicit ShaderBindScope(
          QOpenGLShaderProgram * program, bool was_locked) :
          mWasBound(was_locked) {
        mProgram = program;
        if(!mWasBound) {
          mProgram->bind();
        }
      }

      ~ShaderBindScope() {
        if(!mWasBound) {
          mProgram->release();
        }
      }

    private:
      QOpenGLShaderProgram * mProgram;
      bool mWasBound;
  };


  class QTKAPI MeshRenderer : public Object {
    public:
      // Delegate constructors
      MeshRenderer(
          const char * name, Vertices vertices, Indices indices,
          DrawMode mode = QTK_DRAW_ARRAYS) :
          MeshRenderer(
              name, ShapeBase(mode, std::move(vertices), std::move(indices))) {}

      explicit MeshRenderer(const char * name) :
          MeshRenderer(name, Cube(QTK_DRAW_ELEMENTS)) {}

      // Constructor
      MeshRenderer(const char * name, const ShapeBase & shape);
      ~MeshRenderer() override;

      // Retrieve a mesh by name stored within a static QHash
      static MeshRenderer * getInstance(const QString & name);

      void init();
      void draw();

      // Draw types like GL_TRIANGLES, GL_POINTS, GL_LINES, etc
      void setDrawType(int drawType) { mDrawType = drawType; }

      // Shader settings
      inline void setShaderVertex(const std::string & vert) {
        mVertexShader = vert;
      }

      inline void setShaderFragment(const std::string & frag) {
        mFragmentShader = frag;
      }

      void setShaders(const std::string & vert, const std::string & frag);

      template <typename T> inline void setUniform(int location, T value) {
        ShaderBindScope lock(&mProgram, mBound);
        mProgram.setUniformValue(location, value);
      }

      template <typename T>
      inline void setUniform(const char * location, T value) {
        ShaderBindScope lock(&mProgram, mBound);
        mProgram.setUniformValue(location, value);
      }

      // Set MVP matrix using this Object's transform
      // + View and projection provided by MainWidget static members
      void setUniformMVP(
          const char * model = "uModel", const char * view = "uView",
          const char * projection = "uProjection");

      // These functions modify data stored in a VBO
      // + After calling them, the VBO will need to be reallocated
      void setShape(const Shape & value) override;
      void setColor(const QVector3D & color);

      void setAttributeBuffer(
          int location, GLenum type, int offset, int tupleSize,
          int stride = 0) {
        ShaderBindScope lock(&mProgram, mBound);
        mVAO.bind();
        mProgram.setAttributeBuffer(location, type, offset, tupleSize, stride);
        mVAO.release();
      }

      inline void enableAttributeArray(int location) {
        ShaderBindScope lock(&mProgram, mBound);
        mVAO.bind();
        mProgram.enableAttributeArray(location);
        mVAO.release();
      }

      void reallocateTexCoords(const TexCoords & t, unsigned dims = 2);

      void reallocateNormals(const Normals & n, unsigned dims = 3);

      // Static QHash of all mesh objects within the scene
      typedef QHash<QString, MeshRenderer *> MeshManager;

    private:
      static MeshManager sInstances;

      int mDrawType {};
      std::string mVertexShader {}, mFragmentShader {};
  };
}  // namespace Qtk

#endif  // QTK_MESHRENDERER_H
