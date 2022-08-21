/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Classes for managing objects and data within a scene                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_EXAMPLE_SCENE_H
#define QTK_EXAMPLE_SCENE_H

#include <abstractscene.h>
#include <camera3d.h>
#include <skybox.h>

#include <QMatrix4x4>


class ExampleScene : public Qtk::Scene {
public:
  ExampleScene();
  ~ExampleScene();

  virtual void init();
  virtual void draw() override;
  virtual void update();

private:

  Qtk::MeshRenderer * mTestPhong;
  Qtk::MeshRenderer * mTestSpecular;
  Qtk::MeshRenderer * mTestDiffuse;
  Qtk::MeshRenderer * mTestAmbient;
};

#endif // QTK_EXAMPLE_SCENE_H
