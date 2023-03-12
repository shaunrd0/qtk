/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Collection of static mesh data for quick initialization             ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_SHAPE_H
#define QTK_SHAPE_H

#include <utility>

#include <QOpenGLWidget>
#include <QVector2D>
#include <QVector3D>

#include "qtkapi.h"
#include "transform3D.h"

// Define vertices for drawing a cube using two faces (8 vertex points)
// Front Vertices
#define VERTEX_FTR QVector3D(0.5f, 0.5f, 0.5f)    // 1
#define VERTEX_FTL QVector3D(-0.5f, 0.5f, 0.5f)   // 2
#define VERTEX_FBL QVector3D(-0.5f, -0.5f, 0.5f)  // 3
#define VERTEX_FBR QVector3D(0.5f, -0.5f, 0.5f)   // 4

// Back Vertices
#define VERTEX_BTR QVector3D(0.5f, 0.5f, -0.5f)    // 5
#define VERTEX_BTL QVector3D(-0.5f, 0.5f, -0.5f)   // 6
#define VERTEX_BBL QVector3D(-0.5f, -0.5f, -0.5f)  // 7
#define VERTEX_BBR QVector3D(0.5f, -0.5f, -0.5f)   // 8

// Direction vectors
#define VECTOR_UP      QVector3D(0.0f, 1.0f, 0.0f)
#define VECTOR_DOWN    QVector3D(0.0f, -1.0f, 0.0f)
#define VECTOR_LEFT    QVector3D(-1.0f, 0.0f, 0.0f)
#define VECTOR_RIGHT   QVector3D(1.0f, 0.0f, 0.0f)
#define VECTOR_FORWARD QVector3D(0.0f, 0.0f, 1.0f)
#define VECTOR_BACK    QVector3D(0.0f, 0.0f, -1.0f)

// Identity and zero vectors
#define VECTOR_ONE  QVector3D(1.0f, 1.0f, 1.0f)
#define VECTOR_ZERO QVector3D(0.0f, 0.0f, 0.0f)

// clang-format off
// A series of direction vectors to represent cube face normal
#define FACE_TOP    VECTOR_UP, VECTOR_UP, VECTOR_UP, \
                      VECTOR_UP, VECTOR_UP, VECTOR_UP
#define FACE_BOTTOM VECTOR_DOWN, VECTOR_DOWN, VECTOR_DOWN, \
                      VECTOR_DOWN, VECTOR_DOWN, VECTOR_DOWN
#define FACE_LEFT   VECTOR_LEFT, VECTOR_LEFT, VECTOR_LEFT, \
                      VECTOR_LEFT, VECTOR_LEFT, VECTOR_LEFT
#define FACE_RIGHT  VECTOR_RIGHT, VECTOR_RIGHT, VECTOR_RIGHT, \
                      VECTOR_RIGHT, VECTOR_RIGHT, VECTOR_RIGHT
#define FACE_FRONT  VECTOR_FORWARD, VECTOR_FORWARD, VECTOR_FORWARD, \
                      VECTOR_FORWARD, VECTOR_FORWARD, VECTOR_FORWARD
#define FACE_BACK   VECTOR_BACK, VECTOR_BACK, VECTOR_BACK, \
                      VECTOR_BACK, VECTOR_BACK, VECTOR_BACK
// clang-format on

// Colors using QVector3Ds as RGB values
#define WHITE   VECTOR_ONE
#define BLACK   VECTOR_ZERO
#define RED     QVector3D(1.0f, 0.0f, 0.0f)
#define GREEN   QVector3D(0.0f, 1.0f, 0.0f)
#define BLUE    QVector3D(0.0f, 0.0f, 1.0f)
#define YELLOW  QVector3D(1.0f, 1.0f, 0.0f)
#define CYAN    QVector3D(0.0f, 1.0f, 1.0f)
#define MAGENTA QVector3D(1.0f, 0.0f, 1.0f)

#define UV_ORIGIN QVector2D(0.0f, 0.0f)
#define UV_TOP    QVector2D(1.0f, 0.0f)
#define UV_RIGHT  QVector2D(0.0f, 1.0f)
#define UV_CORNER QVector2D(1.0f, 1.0f)

namespace Qtk {
  class MeshRenderer;

  class Object;

  // TODO: Vertices.getData(); Vertices.getStride();
  typedef std::vector<QVector3D> Vertices;
  typedef std::vector<QVector3D> Colors;
  typedef std::vector<GLuint> Indices;
  typedef std::vector<QVector2D> TexCoords;
  typedef std::vector<QVector3D> Normals;

  /**
   * The OpenGL draw mode to initialize QTK shape data for.
   * Different draw modes require different organization of data.
   * This enum allows us to predefine simple geometry for different draw modes.
   */
  enum DrawMode {
    QTK_DRAW_ARRAYS,
    QTK_DRAW_ELEMENTS,
    QTK_DRAW_ELEMENTS_NORMALS
  };

  /**
   * Base class for all simple shape objects.
   */
  struct QTKAPI ShapeBase {
    public:
      /*************************************************************************
       * Constructors / Destructors
       ************************************************************************/

      /**
       *
       * @param mode OpenGL draw mode to use for this shape.
       * @param v Vertex data for this shape.
       * @param i Index data for this shape.
       * @param c Color data for this shape.
       * @param t Texture coordinates for this shape.
       * @param n Normals for this shape.
       */
      explicit ShapeBase(
          DrawMode mode = QTK_DRAW_ARRAYS, Vertices v = {}, Indices i = {},
          Colors c = {}, TexCoords t = {}, Normals n = {}) :
          mDrawMode(mode),
          mVertices(std::move(v)), mColors(std::move(c)),
          mIndices(std::move(i)), mTexCoords(std::move(t)),
          mNormals(std::move(n)) {}

      /*************************************************************************
       * Accessors
       ************************************************************************/

      /**
       * @return Vertex data for this shape.
       */
      [[nodiscard]] inline const Vertices & getVertices() const {
        return mVertices;
      }

      /**
       * @return Index data for this shape.
       */
      [[nodiscard]] inline const Indices & getIndexData() const {
        return mIndices;
      }

      /**
       * @return Color data for this shape.
       */
      [[nodiscard]] inline const Colors & getColors() const { return mColors; }

      /**
       * @return Texture coordinates for this shape.
       */
      [[nodiscard]] inline const TexCoords & getTexCoords() const {
        return mTexCoords;
      }

      /**
       * @return Normals for this shape.
       */
      [[nodiscard]] inline const Normals & getNormals() const {
        return mNormals;
      }

      /**
       * @return Stride for texture coordinates on this shape.
       */
      [[nodiscard]] inline size_t getTexCoordsStride() const {
        return mTexCoords.size() * sizeof(mTexCoords[0]);
      }

    protected:
      /*************************************************************************
       * Protected Members
       ************************************************************************/

      DrawMode mDrawMode;
      Vertices mVertices {};
      Colors mColors {};
      Indices mIndices {};
      TexCoords mTexCoords {};
      Normals mNormals {};
  };

  struct Shape : public ShapeBase {
    public:
      /*************************************************************************
       * Typedefs
       ************************************************************************/

      friend MeshRenderer;

      friend Object;

      /*************************************************************************
       * Constructors / Destructors
       ************************************************************************/

      Shape() = default;

      explicit Shape(const ShapeBase & rhs) : ShapeBase(rhs) {}

      /*************************************************************************
       * Setters
       ************************************************************************/

      /**
       * @param value Vertex data to use for this shape.
       */
      virtual inline void setVertices(const Vertices & value) {
        mVertices = value;
      }

      /**
       * @param value Index data to use for this shape.
       */
      virtual inline void setIndices(const Indices & value) {
        mIndices = value;
      }

      /**
       * @param value Color data to use for this shape.
       */
      virtual inline void setColors(const Colors & value) { mColors = value; }

      /**
       * @param value Texture coordinates to use for this shape.
       */
      virtual inline void setTexCoords(const TexCoords & value) {
        mTexCoords = value;
      }

      /**
       * @param value Normals to use for this shape.
       */
      virtual inline void setNormals(const Normals & value) {
        mNormals = value;
      }

      /**
       * @param value Shape to copy into this Shape.
       */
      virtual inline void setShape(const Shape & value) { *this = value; }
  };

  /* Simple Cube shape. */
  struct QTKAPI Cube : public ShapeBase {
      explicit Cube(DrawMode mode = QTK_DRAW_ARRAYS);
  };

  /* Simple Triangle shape. */
  struct QTKAPI Triangle : public ShapeBase {
      explicit Triangle(DrawMode mode = QTK_DRAW_ARRAYS);
  };
}  // namespace Qtk

#endif  // QTK_SHAPE_H
