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

Scene::Scene() : mSceneName("Default Scene")
{
  mCamera.getTransform().setTranslation(0.0f, 0.0f, 20.0f);
  mCamera.getTransform().setRotation(-5.0f, 0.0f, 1.0f, 0.0f);
}

Scene::~Scene()
{
  for (auto & mesh : mMeshes) {
    delete mesh;
  }
  for (auto & model : mModels) {
    delete model;
  }
  delete mSkybox;
}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

template <> MeshRenderer * Scene::addObject(MeshRenderer * object)
{
  initSceneObjectName(object);
  mMeshes.push_back(object);
  emit sceneUpdated(mSceneName);
  return object;
}

template <> Model * Scene::addObject(Model * object)
{
  initSceneObjectName(object);
  mModels.push_back(object);
  emit sceneUpdated(mSceneName);
  return object;
}

template <> void Scene::removeObject(MeshRenderer * object)
{
  auto it = std::find(mMeshes.begin(), mMeshes.end(), object);
  if (it == mMeshes.end()) {
    qDebug() << "[Scene::removeObject]: Failed to remove object: "
             << object->getName() << " (" << object << ")";
    return;
  }

  --mObjectCount[object->getName()];
  mMeshes.erase(it);
  emit sceneUpdated(mSceneName);
}

template <> void Scene::removeObject(Model * object)
{
  auto it = std::find(mModels.begin(), mModels.end(), object);
  if (it == mModels.end()) {
    qDebug() << "[Scene::removeObject]: Failed to remove object: "
             << object->getName() << " (" << object << ")";
    return;
  }

  --mObjectCount[object->getName()];
  mModels.erase(it);
  emit sceneUpdated(mSceneName);
}

void Scene::draw()
{
  if (!mInit) {
    initializeOpenGLFunctions();
    init();
    mInit = true;
  }

  // Check if there were new models added that still need to be loaded.
  // This is for objects added at runtime via click-and-drag events, etc.
  while (!mModelLoadQueue.empty()) {
    auto modelSpec = mModelLoadQueue.front();
    // Load the model and add it to the scene.
    addObject(new Model(modelSpec.first.c_str(), modelSpec.second.c_str()));
    mModelLoadQueue.pop();
  }

  if (mPause) {
    return;
  }

  if (mSkybox != Q_NULLPTR) {
    mSkybox->draw();
  }
  for (const auto & model : mModels) {
    model->draw();
  }
  for (const auto & mesh : mMeshes) {
    mesh->draw();
  }
}

std::vector<Object *> Scene::getObjects() const
{
  // All scene objects must inherit from Qtk::Object.
  std::vector<Object *> objects(mMeshes.begin(), mMeshes.end());
  for (const auto & model : mModels) {
    objects.push_back(model);
    if (objects.back() == nullptr) {
      return {};
    }
  }
  return objects;
}

Object * Scene::getObject(const QString & name) const
{
  for (const auto & object : getObjects()) {
    if (object->getName() == name) {
      return object;
    }
  }
  return Q_NULLPTR;
}

void Scene::setSkybox(Skybox * skybox)
{
  delete mSkybox;
  mSkybox = skybox;
}

void Scene::initSceneObjectName(Object * object)
{
  // If the object name exists make it unique.
  auto count = ++mObjectCount[object->getName()];
  if (count > 1) {
    object->setName(object->getName() + " (" + QString::number(count) + ")");
  }
}
