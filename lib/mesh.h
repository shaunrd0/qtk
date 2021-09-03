/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2021 Shaun Reed, all rights reserved                ##
## About: Collection of static mesh data for quick initialization             ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_MESH_H
#define QTK_MESH_H

#include <QOpenGLWidget>
#include <QVector2D>
#include <QVector3D>

#include <mainwidget.h>
#include <transform3D.h>


// Define vertices for drawing a cube using two faces (8 vertex points)
// Front Vertices
#define VERTEX_FTR QVector3D( 0.5f,  0.5f,  0.5f) // 1
#define VERTEX_FTL QVector3D(-0.5f,  0.5f,  0.5f) // 2
#define VERTEX_FBL QVector3D(-0.5f, -0.5f,  0.5f) // 3
#define VERTEX_FBR QVector3D( 0.5f, -0.5f,  0.5f) // 4

// Back Vertices
#define VERTEX_BTR QVector3D( 0.5f,  0.5f, -0.5f) // 5
#define VERTEX_BTL QVector3D(-0.5f,  0.5f, -0.5f) // 6
#define VERTEX_BBL QVector3D(-0.5f, -0.5f, -0.5f) // 7
#define VERTEX_BBR QVector3D( 0.5f, -0.5f, -0.5f) // 8

// Direction vectors
#define VECTOR_UP      QVector3D(0.0f, 1.0f, 0.0f)
#define VECTOR_DOWN    QVector3D(0.0f, -1.0f, 0.0f)
#define VECTOR_LEFT    QVector3D(-1.0f, 0.0f, 0.0f)
#define VECTOR_RIGHT   QVector3D(1.0f, 0.0f, 0.0f)
#define VECTOR_FORWARD QVector3D(0.0f, 0.0f, 1.0f)
#define VECTOR_BACK    QVector3D(0.0f, 0.0f, -1.0f)

// Identity and zero vectors
#define VECTOR_ONE     QVector3D(1.0f, 1.0f, 1.0f)
#define VECTOR_ZERO    QVector3D(0.0f, 0.0f, 0.0f)

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

// Colors using QVector3Ds as RGB values
#define WHITE VECTOR_ONE
#define BLACK VECTOR_ZERO
#define RED   QVector3D(1.0f, 0.0f, 0.0f)
#define GREEN QVector3D(0.0f, 1.0f, 0.0f)
#define BLUE  QVector3D(0.0f, 0.0f, 1.0f)
#define YELLOW  QVector3D(1.0f, 1.0f, 0.0f)
#define CYAN    QVector3D(0.0f, 1.0f, 1.0f)
#define MAGENTA QVector3D(1.0f, 0.0f, 1.0f)

#define UV_ORIGIN  QVector2D(0.0f, 0.0f)
#define UV_TOP     QVector2D(1.0f, 0.0f)
#define UV_RIGHT   QVector2D(0.0f, 1.0f)
#define UV_CORNER  QVector2D(1.0f, 1.0f)


typedef std::vector<QVector3D> Vertices;
typedef std::vector<QVector3D> Colors;
typedef std::vector<GLuint> Indices;
typedef std::vector<QVector2D> TexCoords;
typedef std::vector<QVector3D> Normals;

enum DrawMode { QTK_DRAW_ARRAYS, QTK_DRAW_ELEMENTS, QTK_DRAW_ELEMENTS_NORMALS };

struct ShapeBase {
  ShapeBase(DrawMode mode=QTK_DRAW_ARRAYS, Vertices v={},Indices i={}, Colors c={},
            TexCoords t={}, Normals n={})
      : mVertices(v), mColors(c), mIndices(i), mTexCoords(t), mNormals(n)
  {}

  inline const Vertices & vertices() const { return mVertices;}
  inline const Indices & indices() const { return mIndices;}
  inline const Colors & colors() const { return mColors;}
  inline const TexCoords & texCoords() const { return mTexCoords;}
  inline const Normals & normals() const { return mNormals;}

protected:
  DrawMode mDrawMode;

  Vertices mVertices;
  Colors mColors;
  Indices mIndices;
  TexCoords mTexCoords;
  Normals mNormals;
};

struct Shape : public ShapeBase {
  friend MeshRenderer;
  friend Object;
  Shape () {}
  Shape(const ShapeBase & rhs) : ShapeBase(rhs) {}

  virtual inline void setVertices(const Vertices & value) {mVertices = value;}
  virtual inline void setIndices(const Indices & value) {mIndices = value;}
  virtual inline void setColors(const Colors & value) {mColors = value;}
  virtual inline void setTexCoords(const TexCoords & value) {mTexCoords = value;}
  virtual inline void setNormals(const Normals & value) {mNormals = value;}
  virtual inline void setShape(const Shape & value) { *this = value;}
};

// Primitives inherit from ShapeBase, does not allow setting of shape values
class Mesh {

};

struct Cube : public ShapeBase {
  Cube(DrawMode mode=QTK_DRAW_ARRAYS);
};

struct Triangle : public ShapeBase {
  Triangle(DrawMode mode=QTK_DRAW_ARRAYS);
};

#endif // QTK_MESH_H
