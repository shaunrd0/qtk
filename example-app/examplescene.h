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

class ExampleScene : public Qtk::Scene
{
  public:
    explicit ExampleScene();

    ~ExampleScene();

    /**
     * Override the initialization logic for the scene.
     * This method should up the scene's objects, skybox, etc.
     */
    void init() override;

    /**
     * Optionally override the draw method for the scene.
     *
     * This is just here for example, it should be omitted entirely if we don't
     * want to provide a custom implementation for the ExampleScene.
     */
    void draw() override;

    /**
     * Update objects in the scene for translation or rotation.
     */
    void update() override;
};

#endif  // QTK_EXAMPLE_SCENE_H
