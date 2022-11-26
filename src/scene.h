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
#include <meshrenderer.h>
#include <model.h>
#include <skybox.h>

#include <QMatrix4x4>

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
  class Scene : protected QOpenGLFunctions {
    public:
      /*************************************************************************
       * Contructors / Destructors
       ************************************************************************/

      Scene();

      ~Scene();

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
      virtual void draw() { privateDraw(); };

      /**
       * Function called to update the QOpenGLWidget. Does not trigger a redraw.
       *
       * Calling this several times will still result in only one repaint.
       */
      virtual void update() {}

      /*************************************************************************
       * Accessors
       ************************************************************************/

      static Camera3D & getCamera() { return mCamera; }

      static QMatrix4x4 getViewMatrix() { return mCamera.toMatrix(); }

      static QMatrix4x4 & getProjectionMatrix() { return mProjection; }

      inline Skybox * getSkybox() { return mSkybox; }

      /*************************************************************************
       * Setters
       ************************************************************************/

      inline void setSkybox(Skybox * skybox) { mSkybox = skybox; }

    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/

      static Camera3D mCamera;
      static QMatrix4x4 mProjection;
      bool mInit = false;

      /*************************************************************************
       * Private Methods
       ************************************************************************/

      /**
       * Handles drawing members encapsulated by this base class.
       * Child classes do not need to draw these objects manually.
       */
      void privateDraw();

    protected:
      /*************************************************************************
       * Protected Members
       ************************************************************************/

      /* The skybox for this scene. */
      Skybox * mSkybox {};
      /* MeshRenderers used simple geometry. */
      std::vector<MeshRenderer *> mMeshes {};
      /* Models used for storing 3D models in the scene. */
      std::vector<Model *> mModels {};
  };
}  // namespace Qtk

#endif  // QTK_SCENE_H
