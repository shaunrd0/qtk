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

#include <utility>

#include "camera3d.h"
#include "meshrenderer.h"
#include "model.h"
#include "skybox.h"

namespace Qtk {
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
  class Scene : public QObject, protected QOpenGLFunctions {
      Q_OBJECT

    public:
      /*************************************************************************
       * Contructors / Destructors
       ************************************************************************/

      Scene();

      virtual ~Scene();

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
       *
       * Calling this several times will still result in only one repaint.
       */
      virtual void update() {}

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
      [[nodiscard]] Object * getObject(const QString & name);

      /**
       * @return Camera attached to this scene.
       */
      [[nodiscard]] inline static Camera3D & getCamera() { return mCamera; }

      /**
       * @return View matrix for the camera attached to this scene.
       */
      [[nodiscard]] inline static QMatrix4x4 getViewMatrix() {
        return mCamera.toMatrix();
      }

      /**
       * @return Projection matrix for the current view into the scene.
       */
      [[nodiscard]] inline static QMatrix4x4 & getProjectionMatrix() {
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
      [[nodiscard]] inline const std::vector<MeshRenderer *> & getMeshes()
          const {
        return mMeshes;
      }

      /**
       * @return All Models within the scene.
       */
      [[nodiscard]] inline const std::vector<Model *> & getModels() const {
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

    signals:
      /**
       * Signal thrown when the scene is modified by adding or removing objects.
       * This can be caught by a main application to update any associated data.
       *
       * @param sceneName The scene that has been updated.
       */
      void sceneUpdated(QString sceneName);

    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/

      static Camera3D mCamera;
      static QMatrix4x4 mProjection;
      bool mInit = false;

      QString mSceneName;
      /* The skybox for this scene. */
      Skybox * mSkybox {};
      /* MeshRenderers used simple geometry. */
      std::vector<MeshRenderer *> mMeshes {};
      /* Models used for storing 3D models in the scene. */
      std::vector<Model *> mModels {};
  };

  class SceneEmpty : public Scene {
    public:
      void init() override { setSceneName("Empty Scene"); }

      void draw() override { Scene::draw(); }

      void update() override { Scene::update(); }
  };

  class SceneInterface : public Scene {
    public:
      explicit SceneInterface(Scene * scene) : mScene(scene) {}

      void init() override { mScene->init(); }

      void draw() override { mScene->draw(); }

      void update() override { mScene->update(); }

    protected:
      Scene * mScene;
  };

}  // namespace Qtk

#endif  // QTK_SCENE_H
