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

class ExampleScene : public Qtk::SceneInterface {
  public:
    ExampleScene(Qtk::Scene * scene);

    ~ExampleScene();

    void init() override;

    void draw() override;

    void update() override;
};

#endif  // QTK_EXAMPLE_SCENE_H
