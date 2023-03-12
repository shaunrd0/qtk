/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Example Qtk scene                                                   ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_EXAMPLE_SCENE_H
#define QTK_EXAMPLE_SCENE_H

#include <qtk/scene.h>

/**
 * Example scene using QtkWidget to render 3D models and simple geometry within
 * QtOpenGLWidgets. This scene also shows some examples of using GLSL shaders to
 * apply the basic lighting techniques leading up to Phong.
 *
 * The Qtk::Scene base class provides containers for N pointers to MeshRenderer
 * and Model objects. We can create and insert as many as we like within this
 * child class implementation. This child class does not need to manually draw
 * objects inserted into these containers. The child class would only need to
 * update uniform or other data that may change per-frame.
 * See scene.h and `init()` for more information.
 *
 * To modify the scene objects should be initialized within the `init()` public
 * method. Any required movement or updates should be applied within `draw()` or
 * `update()`.
 *
 * To create your own Scene from scratch see Qtk::Scene.
 */
class QtkScene : public Qtk::SceneInterface {
  public:
    /***************************************************************************
     * Contructors / Destructors
     **************************************************************************/

    QtkScene(Qtk::Scene * scene);

    ~QtkScene();

    /***************************************************************************
     * Inherited Public Overrides
     **************************************************************************/

    /**
     * Initialize objects within the scene
     */
    void init() override;

    /**
     * Called when OpenGL repaints the widget.
     */
    void draw() override;

    /**
     * Called when the Qt `frameSwapped` signal is caught.
     * See definition of `QtkWidget::initializeGL()`
     */
    void update() override;

  private:
    /***************************************************************************
     * Private Members
     **************************************************************************/

    // Additional example objects created within this example.
    // + The base class Scene manages objects stored within mMeshes or mModels
    Qtk::MeshRenderer * mTestPhong {};
    Qtk::MeshRenderer * mTestSpecular {};
    Qtk::MeshRenderer * mTestDiffuse {};
    Qtk::MeshRenderer * mTestAmbient {};
};

#endif  // QTK_EXAMPLE_SCENE_H
