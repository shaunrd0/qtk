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
  class Scene : protected QOpenGLFunctions {
    friend class MainWidget;
  public:
    Scene();
    ~Scene();

    virtual void init() = 0;
    virtual void draw() = 0;
    virtual void update() = 0;

    static Camera3D & Camera() { return mCamera;}
    static QMatrix4x4 View() { return mCamera.toMatrix();}
    static QMatrix4x4 & Projection() { return mProjection;}

    inline Skybox * getSkybox() {return mSkybox;}
    inline void setSkybox(Skybox * skybox) {
      mSkybox = skybox;
    }

  private:
    static Camera3D mCamera;
    static QMatrix4x4 mProjection;
    bool mInit = false;

    void privDraw();

  protected:
    Skybox * mSkybox;
    std::vector<MeshRenderer *> mMeshes;
    std::vector<Model *> mModels;
  };
}

#endif // QTK_SCENE_H
