/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: MeshRenderer class for quick object creation and drawing            ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_MESHRENDERER_H
#define QTK_MESHRENDERER_H

#include <utility>

#include "object.h"
#include "qtkapi.h"
#include "shape.h"

namespace Qtk {
  class QTKAPI MeshRenderer : public Object {
    public:
      /*************************************************************************
       * Typedefs
       ************************************************************************/

      /** Static QHash of all mesh objects within the scene. */
      typedef QHash<QString, MeshRenderer *> MeshManager;

      /*************************************************************************
       * Constructors / Destructors
       ************************************************************************/

      /**
       * Delegate constructor.
       * Constructs a MeshRenderer with custom vertices and indices for more
       * complex geometry.
       *
       * @param name Name to use for the new QObject.
       * @param vertices Vertices to use for initializing geometry shape.
       * @param indices Indicess to use for initializes geometry shape.
       * @param mode OpenGL draw mode. Supported modes are prefixed with QTK_*
       */
      MeshRenderer(
          const char * name, Vertices vertices, Indices indices,
          DrawMode mode = QTK_DRAW_ARRAYS);

      /**
       * Delegate constructor.
       * Constructs a MeshRenderer with a default shape of a cube.
       *
       * @param name Name to use for the new QObject.
       */
      explicit MeshRenderer(const char * name);

      /**
       * Construct a MeshRenderer.
       * Default shaders will be used unless subsequently set by the caller.
       *
       * @param name Name to use for the new QObject.
       * @param shape The shape of the MeshRenderer.
       *    For models this can be set using ShapeBase ctors.
       */
      MeshRenderer(const char * name, const ShapeBase & shape);

      ~MeshRenderer();

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      /**
       * Initializes OpenGL buffers and settings for this MeshRenderer.
       */
      void init();

      /**
       * Draws this MeshRenderer.
       */
      void draw();

      /**
       * Enables shader attribute array from the MeshRenderer's VAO.
       * @param location Index location of the attribute array to enable.
       */
      void enableAttributeArray(int location);

      /**
       * Reallocates texture coordinates to the mNBO member object.
       *
       * @param t Texture coordinates to reallocate.
       * @param dims Number of dimensions to use for the coordinates.
       */
      void reallocateTexCoords(const TexCoords & t, unsigned dims = 2);

      /**
       * Reallocates normals to the mNBO member object.
       *
       * @param n Normal coordinate to reallocate.
       * @param dims Number of dimensions to use for the coordinates.
       */
      void reallocateNormals(const Normals & n, unsigned dims = 3);

      /*************************************************************************
       * Setters
       ************************************************************************/

      /**
       * Set OpenGL draw type. GL_TRIANGLES, GL_POINTS, GL_LINES, etc.
       *
       * @param drawType The draw type to use for this MeshRenderer.
       */
      inline void setDrawType(int drawType) { mDrawType = drawType; }

      /**
       * @param vert Path to vertex shader to use for this MeshRenderer.
       */
      inline void setShaderVertex(const std::string & vert) {
        mVertexShader = vert;
      }

      /**
       * @param frag Path to fragment shader to use for this MeshRenderer.
       */
      inline void setShaderFragment(const std::string & frag) {
        mFragmentShader = frag;
      }

      /**
       * @param vert Path to vertex shader to use for this MeshRenderer.
       * @param frag Path to fragment shader to use for this MeshRenderer.
       */
      void setShaders(const std::string & vert, const std::string & frag);

      /**
       * @tparam T Type of the uniform value to set.
       * @param location Index location of the uniform value we are setting.
       * @param value The value to use for the uniform.
       */
      template <typename T> inline void setUniform(int location, T value) {
        ShaderBindScope lock(&mProgram, mBound);
        mProgram.setUniformValue(location, value);
      }

      /**
       * @tparam T Type of the uniform value to set.
       * @param location Name of the uniform value we are setting.
       * @param value The value to use for the uniform.
       */
      template <typename T>
      inline void setUniform(const char * location, T value) {
        ShaderBindScope lock(&mProgram, mBound);
        mProgram.setUniformValue(location, value);
      }

      /**
       * Sets the MVP matrices for this object within the scene.
       * Model matrix is provided by this model's transform.
       * View and Projection matrices are provided by the scene.
       *
       * @param model Name of the uniform to store the Model matrix.
       * @param view Name of the uniform to store the View matrix.
       * @param projection Name of the uniform to store the Projection matrix.
       */
      void setUniformMVP(
          const char * model = "uModel", const char * view = "uView",
          const char * projection = "uProjection");

      /**
       * Sets the shape of the MeshRenderer using the Object base class method.
       * The MeshRenderer will be reinitialized after this call using `init()`.
       *
       * @param value Shape to use for this MeshRenderer.
       */
      void setShape(const Shape & value) override;

      /**
       * Sets all vertices in the mesh to a color.
       * The MeshRenderer will be reinitialized after this call using `init()`.
       *
       * @param color The color to use for the entire mesh.
       */
      void setColor(const QVector3D & color);

      /**
       * Updates an attribute buffer. This should be called whenever related
       * buffers are reallocated. If the new buffer uses an identical format
       * this may not be required.
       *
       * @param location Index location of the attribute buffer to set.
       * @param type The type of the values within the attribute buffer.
       * @param offset Offset to the beginning of the buffer.
       * @param tupleSize Size of each group of elements in the buffer.
       *    For (x, y) positions this would be 2, (x, y, z) would be 3, etc.
       * @param stride Stride between groups of elements in the buffer.
       *    For example (x, y) data stride is `2 * sizeof(type)`
       */
      void setAttributeBuffer(
          int location, GLenum type, int offset, int tupleSize, int stride = 0);

      /*************************************************************************
       * Accessors
       ************************************************************************/

      /**
       * Retrieve a mesh by name stored within static QHash private member
       * @param name The name of the MeshRenderer we want to retrieve.
       * @return Pointer to the MeshRenderer, or nullptr if not found.
       */
      static MeshRenderer * getInstance(const QString & name);

      /**
       * @return Transform3D attached to this MeshRenderer.
       */
      inline Transform3D & getTransform() { return mTransform; }

    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/

      static MeshManager sInstances;

      int mDrawType {};
      std::string mVertexShader {}, mFragmentShader {};
  };
}  // namespace Qtk

#endif  // QTK_MESHRENDERER_H
