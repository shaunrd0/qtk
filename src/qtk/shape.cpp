/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Collection of static mesh data for quick initialization             ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "shape.h"

using namespace Qtk;

Cube::Cube(DrawMode mode) {
  mDrawMode = mode;
  switch(mode) {
    // The order of the following assignment values helps to visualize.
    // clang-format off

    // Cube data for use with glDrawArrays
    case QTK_DRAW_ARRAYS:
      mIndices = {/* No indices needed for glDrawArrays */};

      mNormals = {FACE_FRONT,  FACE_BACK, FACE_TOP,
                  FACE_BOTTOM, FACE_LEFT, FACE_RIGHT};

      // We're drawing triangles to construct the geometry of a cube.
      // Each triangle is made up of three points.
      // The entire cube has 12 triangles to make 6 square faces of the cube.
      mVertices = {
          // Face 1 (Front)
          VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
          VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
          // Face 2 (Back)
          VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
          VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
          // Face 3 (Top)
          VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
          VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
          // Face 4 (Bottom)
          VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
          VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
          // Face 5 (Left)
          VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
          VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
          // Face 6 (Right)
          VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
          VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
      };

      // Setting colors for each vertex that we defined above.
      // These are defaults and can be overriden by the caller with setColor().
      // The colors below are interpolated from vertex to vertex.
      mColors = {
          // Face 1 (Front)
          RED, GREEN, BLUE,
          BLUE, WHITE, RED,
          // Face 2 (Back)
          YELLOW, CYAN, MAGENTA,
          CYAN, YELLOW, BLACK,
          // Face 3 (Top)
          RED, MAGENTA, CYAN,
          CYAN, GREEN, RED,
          // Face 4 (Bottom)
          WHITE, BLUE, BLACK,
          BLACK, YELLOW, WHITE,
          // Face 5 (Left)
          BLUE, GREEN, CYAN,
          BLUE, CYAN, BLACK,
          // Face 6 (Right)
          RED, WHITE, YELLOW,
          YELLOW, MAGENTA, RED
      };

      // Define texture coordinates for the cube.
      // This defines the orientation of the texture when applied the object.
      mTexCoords = {
          // Face 1 (Front)
          UV_TOP, UV_ORIGIN, UV_RIGHT,
          UV_RIGHT, UV_CORNER, UV_TOP,
          // Face 2 (Back)
          UV_TOP, UV_RIGHT, UV_CORNER,
          UV_RIGHT, UV_TOP, UV_ORIGIN,
          // Face 3 (Top)
          UV_CORNER, UV_TOP, UV_ORIGIN,
          UV_ORIGIN, UV_RIGHT, UV_CORNER,
          // Face 4 (Bottom)
          UV_TOP, UV_ORIGIN, UV_RIGHT,
          UV_RIGHT, UV_CORNER, UV_TOP,
          // Face 5 (Left)
          UV_TOP, UV_CORNER, UV_RIGHT,
          UV_TOP, UV_RIGHT, UV_ORIGIN,
          // Face 6 (Right)
          UV_TOP, UV_CORNER, UV_RIGHT,
          UV_RIGHT, UV_ORIGIN, UV_TOP
      };

      break;

      // Cube data for use with glDrawElements
    case QTK_DRAW_ELEMENTS:
      mNormals = {
          /* For normals and glDrawElements, see QTK_DRAW_ELEMENTS_NORMALS */};
      mTexCoords = {
          /* For UVs and glDrawElements, see QTK_DRAW_ELEMENTS_NORMALS */};

      mColors = {RED, GREEN, BLUE, WHITE, YELLOW, CYAN, MAGENTA, BLACK};

      mVertices = {//       0           1           2           3
                   VERTEX_FTR, VERTEX_FTL, VERTEX_FBL, VERTEX_FBR,
                   //       4           5           6           7
                   VERTEX_BTR, VERTEX_BTL, VERTEX_BBL, VERTEX_BBR};

      mIndices = {
          // Face 1 (Front)
          0, 1, 2,    2, 3, 0,
          // Face 2 (Back)
          7, 5, 4,    5, 7, 6,
          // Face 3 (Top)
          0, 4, 5,    5, 1, 0,
          // Face 4 (Bottom)
          3, 2, 6,    6, 7, 3,
          // Face 5 (Left)
          2, 1, 5,    2, 5, 6,
          // Face 6 (Right)
          0, 3, 7,    7, 4, 0
      };

      break;

      // Cube shape data for using normals and UVs with glDrawElements
    case QTK_DRAW_ELEMENTS_NORMALS:
      mColors = {RED, GREEN, BLUE, WHITE, YELLOW, CYAN, MAGENTA, BLACK};

      mVertices = {
          // Face 1 (Front)
          //       0           1           2           3
          VERTEX_FTL, VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
          // Face 2 (Back)
          //       4           5           6           7
          VERTEX_BTL, VERTEX_BBL, VERTEX_BBR, VERTEX_BTR,
          // Face 3 (Top)
          //       8           9          10          11
          VERTEX_FTL, VERTEX_BTL, VERTEX_BTR, VERTEX_FTR,
          // Face 4 (Bottom)
          //      12          13          14          15
          VERTEX_FBL, VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
          // Face 5 (Left)
          //      16          17          18          19
          VERTEX_FBL, VERTEX_BBL, VERTEX_BTL, VERTEX_FTL,
          // Face 6 (Right)
          //      20          21          22          23
          VERTEX_FBR, VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
      };

      mIndices = {
          // Face 1 (Front)
          0, 1, 2,    2, 3, 0,
          // Face 2 (Back)
          4, 5, 6,    6, 7, 4,
          // Face 3 (Top)
          8, 9, 10,    10, 11, 8,
          // Face 4 (Bottom)
          12, 13, 14,    14, 15, 12,
          // Face 5 (Left)
          16, 17, 18,    18, 19, 16,
          // Face 6 (Right)
          20, 21, 22,    22, 23, 20
      };


      mNormals = {
          VECTOR_FORWARD, VECTOR_FORWARD, VECTOR_FORWARD, VECTOR_FORWARD,
          VECTOR_BACK,    VECTOR_BACK,    VECTOR_BACK,    VECTOR_BACK,
          VECTOR_UP,      VECTOR_UP,      VECTOR_UP,      VECTOR_UP,
          VECTOR_DOWN,    VECTOR_DOWN,    VECTOR_DOWN,    VECTOR_DOWN,
          VECTOR_LEFT,    VECTOR_LEFT,    VECTOR_LEFT,    VECTOR_LEFT,
          VECTOR_RIGHT,   VECTOR_RIGHT,   VECTOR_RIGHT,   VECTOR_RIGHT,
      };

      mTexCoords = {
          // Face 1 (Front)
          UV_TOP, UV_RIGHT, UV_CORNER,
          UV_RIGHT, UV_TOP, UV_ORIGIN,
          // Face 2 (Back)
          UV_TOP, UV_RIGHT, UV_CORNER,
          UV_RIGHT, UV_TOP, UV_ORIGIN,
          // Face 3 (Top)
          UV_TOP, UV_RIGHT, UV_CORNER,
          UV_RIGHT, UV_TOP, UV_ORIGIN,
          // Face 4 (Bottom)
          UV_TOP, UV_RIGHT, UV_CORNER,
          UV_RIGHT, UV_TOP, UV_ORIGIN,
          // Face 5 (Left)
          UV_TOP, UV_RIGHT, UV_CORNER,
          UV_RIGHT, UV_TOP, UV_ORIGIN,
          // Face 6 (Right)
          UV_TOP, UV_RIGHT, UV_CORNER,
          UV_RIGHT, UV_TOP, UV_ORIGIN,
      };

      break;
      // clang-format on
  }
}

Triangle::Triangle(DrawMode mode) {
  mDrawMode = mode;
  const QVector3D triangleTop = QVector3D(0.0f, 0.5f, 0.0f);
  switch(mode) {
      // clang-format off

    case QTK_DRAW_ARRAYS:
      mIndices = {/* No indices needed for glDrawArrays */};

      mColors = {RED, GREEN, BLUE, WHITE, YELLOW, CYAN, MAGENTA, BLACK};

      mVertices = {
          // Bottom face (Base of the pyramid)
          VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
          VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,

          // Front face
          VERTEX_FBL, VERTEX_FBR, triangleTop,
          // Back face
          VERTEX_BBR, VERTEX_BBL, triangleTop,
          // Left face
          VERTEX_BBL, VERTEX_FBL, triangleTop,
          // Right face
          VERTEX_FBR, VERTEX_BBR, triangleTop,
      };

      // Find normals for each triangle of the mesh
      for(int i = 0; i < mVertices.size(); i += 3) {
        QVector3D vertexNormal =
            QVector3D::normal(mVertices[i], mVertices[i + 1], mVertices[i + 2]);
        // Three points share this normal
        for(int j = 0; j < 3; j++) {
          mNormals.push_back(vertexNormal);
        }
      }

      mTexCoords = {
          // Bottom face (Base of the pyramid)
          UV_ORIGIN, UV_RIGHT, UV_CORNER,
          UV_CORNER, UV_TOP, UV_ORIGIN,

          // Front face
          UV_ORIGIN, UV_RIGHT, UV_CORNER,
          // Back face
          UV_ORIGIN, UV_RIGHT, UV_CORNER,
          // Left face
          UV_ORIGIN, UV_RIGHT, UV_CORNER,
          // Right face
          UV_ORIGIN, UV_RIGHT, UV_CORNER,
      };

      break;

      // Triangle shape data for using glDrawElements
    case QTK_DRAW_ELEMENTS:
      mColors = {RED, GREEN, BLUE, WHITE, YELLOW, CYAN, MAGENTA, BLACK};

      mVertices = {VERTEX_FBL, VERTEX_FBR, VERTEX_BBL, VERTEX_BBR, triangleTop};

      mIndices = {
          // Bottom face (Base of the pyramid)
          2, 3, 1,  // Use customVertexes[2], then 3, 1...
          1, 0, 2,  // Use customVertexes[1], then 0, 2

          0, 1, 4,  // Front face
          3, 2, 4,  // Back face
          2, 0, 4,  // Left face
          1, 3, 4,  // Right face
      };

      mNormals = {
          /* Use QTK_DRAW_ELEMENTS_NORMALS for normals with glDrawElements */};

      mTexCoords = {/* No UVs for triangle with glDrawElements */};

      break;

      // Triangle shape data for using normals and UVs with glDrawElements
    case QTK_DRAW_ELEMENTS_NORMALS:
      mColors = {RED, GREEN, BLUE, WHITE, YELLOW, CYAN, MAGENTA, BLACK};

      mVertices = {
          // Bottom face
          //       0           1           2
          VERTEX_FBL, VERTEX_FBR, VERTEX_BBL,
          //       3            4          5
          VERTEX_BBR, VERTEX_FBR, VERTEX_BBL,

          // Front face
          //       6           7           8
          VERTEX_FBL, VERTEX_FBR, triangleTop,
          // Back face
          //       9           10        11
          VERTEX_BBR, VERTEX_BBL, triangleTop,
          // Left face
          //      12          13          14
          VERTEX_BBL, VERTEX_FBL, triangleTop,
          // Right face
          //      15           16         17
          VERTEX_FBR, VERTEX_BBR, triangleTop,
      };

      mIndices = {
          // Bottom face (Base of the pyramid)
          0,  1,  2,  // Use customVertexes[2], then 3, 1...
          3,  4,  5,  // Use customVertexes[1], then 0, 2

          6,  7,  8,   // Front face
          9,  10, 11,  // Back face
          12, 13, 14,  // Left face
          15, 16, 17,  // Right face
      };

      // Find normals for each triangle of the mesh
      for(int i = 0; i < mVertices.size(); i += 3) {
        QVector3D vertexNormal = QVector3D::normal(
            mVertices[mIndices[i]], mVertices[mIndices[i + 1]],
            mVertices[mIndices[i + 2]]);
        // Three points share this normal
        for(int j = 0; j < 3; j++) {
          mNormals.push_back(vertexNormal);
        }
      }

      mTexCoords = {
          // Bottom face
          UV_ORIGIN, UV_RIGHT, UV_TOP,
          UV_CORNER, UV_RIGHT, UV_TOP,

          // Front face
          UV_ORIGIN, UV_RIGHT, UV_CORNER,
          // Back face
          UV_ORIGIN, UV_RIGHT, UV_CORNER,
          // Left face
          UV_ORIGIN, UV_RIGHT, UV_CORNER,
          // Right face
          UV_ORIGIN, UV_RIGHT, UV_CORNER,
      };
      break;
      // clang-format on
  }
}
