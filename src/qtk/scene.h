/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Classes for managing objects and data within a scene                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_SCENE_H
#define QTK_SCENE_H

#include <QMatrix4x4>
#include <QUrl>

#include <queue>
#include <unordered_map>
#include <utility>

#include "camera3d.h"
#include "meshrenderer.h"
#include "model.h"
#include "skybox.h"

namespace Qtk
{
  /**
   * An abstract Scene class to inherit from when building new scenes.
   *
   * This class provides the following objects to any inheriting scene:
   *    Skybox, Camera
   * This class also provides containers for N instances of these objects:
   *    MeshRenderers, Models
   *
   * To inherit from this class and define our own scene we must:
   *
   * Override and define the `init()` virtual member function. If we want our
   * scene to render using a Skybox, we should also initialize the mSkybox
   * member within the overridden definition of `init()` using
   * `Scene::setSkybox(...)`
   *
   * If the scene is to render any kind of movement we are required to override
   * the `update()` virtual method.
   *
   * If the child scene adds any objects which are not managed (drawn) by this
   * base class, the child scene class must also override the `draw()` method.
   */
  class Scene : public QObject, protected QOpenGLFunctions
  {
      Q_OBJECT

    public:
      /*************************************************************************
       * Contructors / Destructors
       ************************************************************************/

      Scene();

      ~Scene() override;

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      /**
       * Initialize objects within the scene
       */
      virtual void init() = 0;

      /**
       * Function called during OpenGL drawing event.
       *
       * This function is only called when the widget is redrawn.
       */
      virtual void draw();

      /**
       * Function called to update the QOpenGLWidget. Does not trigger a redraw.
       * This method can translate or rotate objects to simulate movement.
       * Calling this several times will still result in only one repaint.
       *
       * It's very possible a client will not want to move objects in the scene
       * using this method. This is intentially not pure virtual.
       */
      virtual void update() {}

      void loadModel(const QUrl & url)
      {
        auto fileName = url.fileName().replace(".obj", "").toStdString();
        auto filePath = url.toLocalFile().toStdString();
        loadModel(fileName, filePath);
      }

      void loadModel(const std::string & name, const std::string & path)
      {
        // Add the dropped model to the load queue.
        // This is consumed during rendering of the scene if not empty.
        mModelLoadQueue.emplace(name, path);
      }

      /*************************************************************************
       * Accessors
       ************************************************************************/

      /**
       * @return All Qtk::Objects within the scene.
       *    If any object is invalid, we return an empty vector.
       */
      [[nodiscard]] std::vector<Object *> getObjects() const;

      /**
       * Retrieve and object from the scene by it's objectName.
       *
       * @param name The objectName to look for within this scene.
       * @return The found object or Q_NULLPTR if none found.
       */
      [[nodiscard]] Object * getObject(const QString & name) const;

      /**
       * @return The number of objects within the scene with the given name.
       */
      [[nodiscard]] uint64_t getObjectCount(const QString & name)
      {
        return mObjectCount.count(name) ? mObjectCount[name] : 0;
      }

      /**
       * @return Camera attached to this scene.
       */
      [[nodiscard]] inline static Camera3D & getCamera() { return mCamera; }

      /**
       * @return View matrix for the camera attached to this scene.
       */
      [[nodiscard]] inline static QMatrix4x4 getViewMatrix()
      {
        return mCamera.toMatrix();
      }

      /**
       * @return Projection matrix for the current view into the scene.
       */
      [[nodiscard]] inline static QMatrix4x4 & getProjectionMatrix()
      {
        return mProjection;
      }

      /**
       * @return The active skybox for this scene.
       */
      [[nodiscard]] inline Skybox * getSkybox() { return mSkybox; }

      /**
       * @return The name for this scene. This is entirely user defined and not
       *    a Qt objectName.
       */
      [[nodiscard]] inline QString getSceneName() const { return mSceneName; }

      /**
       * @return All MeshRenderers within the scene.
       */
      [[nodiscard]] inline const std::vector<MeshRenderer *> & getMeshes() const
      {
        return mMeshes;
      }

      /**
       * @return All Models within the scene.
       */
      [[nodiscard]] inline const std::vector<Model *> & getModels() const
      {
        return mModels;
      }

      /*************************************************************************
       * Setters
       ************************************************************************/

      /**
       * @param skybox New skybox to use for this scene.
       */
      void setSkybox(Skybox * skybox);

      /**
       * Adds objects to the scene.
       * This template provides explicit specializations for valid types.
       * Adding any object other than these types will cause errors.
       * TODO: Refactor to use Object base class container for scene objects.
       *
       * If creating a new object type for a scene, it must inherit Qtk::Object
       * and provide a specialization for this method.
       *
       * @param object The new object to add to the scene.
       * @return The object added to the scene.
       */
      template <typename T> T * addObject(T * object);

      /**
       * @param name The name to use for this scene.
       */
      inline void setSceneName(QString name) { mSceneName = std::move(name); }

      inline void setPause(bool pause) { mPause = pause; }

    signals:
      /**
       * Signal thrown when the scene is modified by adding or removing objects.
       * This can be caught by a main application to update any associated data.
       *
       * @param sceneName The scene that has been updated.
       */
      void sceneUpdated(QString sceneName);


      /*************************************************************************
       * Public Members
       ************************************************************************/
    public:
      /* Models used for storing 3D models in the scene. */
      std::vector<Model *> mModels {};

      /* Queue of models requested to load at runtime. */
      std::queue<std::pair<std::string, std::string>> mModelLoadQueue;

    private:
      /**
       * Initialize an object name relative to other objects already loaded.
       * Protects against having two objects with the same name.
       *
       * @param object Qtk Object to name within this scene.
       */
      void initSceneObjectName(Qtk::Object * object);

      /*************************************************************************
       * Private Members
       ************************************************************************/

      static Camera3D mCamera;
      static QMatrix4x4 mProjection;
      bool mInit = false;
      /* Pause rendering of the scene. */
      bool mPause = false;

      QString mSceneName;
      /* The skybox for this scene. */
      Skybox * mSkybox {};
      /* MeshRenderers used simple geometry. */
      std::vector<MeshRenderer *> mMeshes {};
      /* Track count of objects with same initial name. */
      std::unordered_map<QString, uint64_t> mObjectCount;
  };
}  // namespace Qtk

#endif  // QTK_SCENE_H
