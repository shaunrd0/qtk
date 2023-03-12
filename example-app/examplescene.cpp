/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Example Qtk scene                                                   ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "examplescene.h"

using namespace Qtk;

ExampleScene::ExampleScene(Qtk::Scene * scene) : Qtk::SceneInterface(scene) {
  setSceneName("Example Scene");
  getCamera().getTransform().setTranslation(-8.0f, 0.0f, 10.0f);
  getCamera().getTransform().setRotation(-5.0f, 0.0f, 1.0f, 0.0f);
}

ExampleScene::~ExampleScene() {}

void ExampleScene::init() {
  auto skybox = new Qtk::Skybox("Skybox");
  setSkybox(skybox);

  auto spartan = new Model(
      "spartan", "/home/kapper/Code/qtk/resources/models/spartan/spartan.obj");
  addObject(spartan);

  auto mesh = addObject(
      new Qtk::MeshRenderer("rightTriangle", Triangle(QTK_DRAW_ARRAYS)));
  mesh->getTransform().setTranslation(-5.0f, 0.0f, -2.0f);

  // QTK_DRAW_ARRAYS is the default for generic shapes in qtk/shape.h
  addObject(new Qtk::MeshRenderer("centerCube", Cube(QTK_DRAW_ARRAYS)))
      ->getTransform()
      .setTranslation(-7.0f, 0.0f, -2.0f);

  mesh = addObject(
      new Qtk::MeshRenderer("leftTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(-9.0f, 0.0f, -2.0f);
  mesh->setDrawType(GL_LINE_LOOP);

  mesh = addObject(
      new Qtk::MeshRenderer("topTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(-7.0f, 2.0f, -2.0f);
  mesh->getTransform().scale(0.25f);

  mesh = addObject(
      new Qtk::MeshRenderer("bottomTriangle", Triangle(QTK_DRAW_ELEMENTS)));
  mesh->getTransform().setTranslation(-7.0f, -2.0f, -2.0f);
  mesh->getTransform().scale(0.25f);
  mesh->setDrawType(GL_LINE_LOOP);
  mesh->setColor(GREEN);
}

void ExampleScene::draw() {
  Scene::draw();
}

void ExampleScene::update() {
  // Pitch forward and roll sideways
  MeshRenderer::getInstance("leftTriangle")
      ->getTransform()
      .rotate(0.75f, 1.0f, 0.0f, 0.0f);
  MeshRenderer::getInstance("rightTriangle")
      ->getTransform()
      .rotate(0.75f, 0.0f, 0.0f, 1.0f);

  static float translateX = 0.025f;
  float limit = -9.0f;  // Origin position.x - 2.0f
  float posX = MeshRenderer::getInstance("topTriangle")
                   ->getTransform()
                   .getTranslation()
                   .x();
  if(posX < limit || posX > limit + 4.0f) {
    translateX = -translateX;
  }
  MeshRenderer::getInstance("topTriangle")
      ->getTransform()
      .translate(translateX, 0.0f, 0.0f);
  MeshRenderer::getInstance("bottomTriangle")
      ->getTransform()
      .translate(-translateX, 0.0f, 0.0f);
  MeshRenderer::getInstance("topTriangle")
      ->getTransform()
      .rotate(0.75f, 0.2f, 0.0f, 0.4f);
  MeshRenderer::getInstance("bottomTriangle")
      ->getTransform()
      .rotate(0.75f, 0.0f, 0.2f, 0.4f);

  MeshRenderer::getInstance("centerCube")
      ->getTransform()
      .rotate(0.75f, 0.2f, 0.4f, 0.6f);
}
