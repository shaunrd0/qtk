/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Classes for managing objects and data within a scene                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <camera3d.h>
#include <resourcemanager.h>
#include <scene.h>
#include <texture.h>

using namespace Qtk;

Camera3D Scene::mCamera;
QMatrix4x4 Scene::mProjection;

/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

Scene::Scene() {
  mCamera.getTransform().setTranslation(0.0f, 0.0f, 20.0f);
  mCamera.getTransform().setRotation(-5.0f, 0.0f, 1.0f, 0.0f);
}

Scene::~Scene() {
  for(auto & mesh : mMeshes) {
    delete mesh;
  }
  for(auto & model : mModels) {
    delete model;
  }
  delete mSkybox;
}

void Scene::privateDraw() {
  if(!mInit) {
    initializeOpenGLFunctions();
    init();
    mInit = true;
  }
  if(mSkybox != Q_NULLPTR) {
    mSkybox->draw();
  }
  for(auto & model : mModels) {
    model->draw();
  }
  for(const auto & mesh : mMeshes) {
    mesh->draw();
  }
}
