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


class MeshRenderer : public Object {
public:
  // Delegate constructors
  MeshRenderer(const char * name, Vertices vertices, Indices indices,
               DrawMode mode=QTK_DRAW_ARRAYS)
      : MeshRenderer(name, ShapeBase(mode, vertices, indices))
  {}
  MeshRenderer(const char * name)
      : MeshRenderer(name, Cube(QTK_DRAW_ELEMENTS))
  {}
  // Constructor
  MeshRenderer(const char * name, const ShapeBase &shape);
  ~MeshRenderer();

  // Retrieve a mesh by name stored within a static QHash
  static MeshRenderer * getInstance(const QString & name);

  void init();
  void draw();

  // Draw types like GL_TRIANGLES, GL_POINTS, GL_LINES, etc
  void setDrawType(int drawType) { mDrawType = drawType;}

  // Shader settings
  inline void setShaderVertex(const std::string & vert) { mVertexShader = vert;}
  inline void setShaderFragment(const std::string & frag)
  { mFragmentShader = frag;}
  void setShaders(const std::string & vert, const std::string & frag);

  template <typename T>
  inline void setUniform(int location, T value)
  { mProgram.setUniformValue(location, value);}

  template <typename T>
  inline void setUniform(const char * location, T value)
  { mProgram.setUniformValue(location, value);}

  // Set MVP matrix using this Object's transform
  // + View and projection provided by MainWidget static members
  void setUniformMVP(const char * model="uModel", const char * view="uView",
                     const char * projection="uProjection");

  // Sets the texture to the image at the given path
  // + Sets mHasTexture to enable texture binding in draw()
  void setTexture(const char * path);
  void setTexture(QOpenGLTexture * texture);

  // These functions modify data stored in a VBO
  // + After calling them, the VBO will need to be reallocated
  void setShape(const Shape & value) override;
  void setColor(const QVector3D & color);

  // Static QHash of all mesh objects within the scene
  typedef QHash<QString, MeshRenderer *> MeshManager;
private:
  static MeshManager sInstances;

  int mDrawType;
  bool mHasTexture;
  std::string mVertexShader, mFragmentShader;
};

#endif // QTK_MESHRENDERER_H
