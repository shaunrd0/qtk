/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Classes for managing objects and data within a scene                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "scene.h"
#include "camera3d.h"

using namespace Qtk;

Camera3D Scene::mCamera;
QMatrix4x4 Scene::mProjection;

/*******************************************************************************
 * Constructors / Destructors
 ******************************************************************************/

Scene::Scene() : mSceneName("Default Scene") {
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

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

void Scene::draw() {
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

std::vector<Object *> Scene::getObjects() const {
  // All scene objects must inherit from Qtk::Object.
  std::vector<Object *> objects(mMeshes.begin(), mMeshes.end());
  for(auto model : mModels) {
    objects.push_back(dynamic_cast<Object *>(model));
    if(objects.back() == nullptr) {
      return {};
    }
  }
  return objects;
}

Object * Scene::getObject(const QString & name) {
  for(auto object : getObjects()) {
    if(object->getName() == name.toStdString()) {
      return object;
    }
  }
  return Q_NULLPTR;
}

void Scene::setSkybox(Skybox * skybox) {
  delete mSkybox;
  mSkybox = skybox;
}

template <> MeshRenderer * Scene::addObject(MeshRenderer * object) {
  mMeshes.push_back(object);
  sceneUpdated(mSceneName);
  return object;
}

template <> Model * Scene::addObject(Model * object) {
  mModels.push_back(object);
  sceneUpdated(mSceneName);
  return object;
}
