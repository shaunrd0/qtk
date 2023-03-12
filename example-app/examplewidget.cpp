/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Example Qtk widget                                                  ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <qtk/scene.h>

#include "examplewidget.h"

ExampleWidget::ExampleWidget(QWidget * parent) :
    QOpenGLWidget(parent), mScene(new ExampleScene(new Qtk::SceneEmpty)) {
  // NOTE: The decorator pattern is used to save / load scenes in Qtk currently.
  // The initializer above sets mScene to the concrete decorator ExampleScene.
  // Qtk::SceneEmpty provides an empty scene as the concrete component.
  // ExampleScene is defined in client source, deriving Qtk::SceneInterface.

  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 6);
  format.setSamples(4);
  format.setDepthBufferSize(16);
  setFormat(format);
  setFocusPolicy(Qt::ClickFocus);
}

void ExampleWidget::initializeGL() {
  initializeOpenGLFunctions();
  connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.1f, 1.0f);
  glClearDepth(1.0f);
  glClearColor(0.0f, 0.25f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ExampleWidget::resizeGL(int width, int height) {
  Qtk::Scene::getProjectionMatrix().setToIdentity();
  Qtk::Scene::getProjectionMatrix().perspective(
      45.0f, float(width) / float(height), 0.1f, 1000.0f);
}

void ExampleWidget::paintGL() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  mScene->draw();
}

void ExampleWidget::update() {
  mScene->update();
  QWidget::update();
}
