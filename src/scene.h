/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Classes for managing objects and data within a scene                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_SCENE_H
#define QTK_SCENE_H

#include <camera3d.h>
#include <skybox.h>

#include <QMatrix4x4>

class MeshRenderer;
class Model;

class Scene {
public:
  Scene();
  ~Scene();

  void init();
  void draw();
  void update();

  static Camera3D & Camera() { return mCamera;}
  static QMatrix4x4 View() { return mCamera.toMatrix();}
  static QMatrix4x4 & Projection() { return mProjection;}
private:
  static Camera3D mCamera;
  static QMatrix4x4 mProjection;

  Skybox mSkybox;
  MeshRenderer * mTestPhong;
  MeshRenderer * mTestSpecular;
  MeshRenderer * mTestDiffuse;
  MeshRenderer * mTestAmbient;
  std::vector<MeshRenderer *> mMeshes;
  std::vector<Model *> mModels;
};

#endif // QTK_SCENE_H
