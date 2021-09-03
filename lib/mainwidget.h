/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2021 Shaun Reed, all rights reserved                ##
## About: Main window for Qt5 OpenGL widget application                       ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_MAINWIDGET_H
#define QTK_MAINWIDGET_H

#include <iostream>

#include <QMatrix4x4>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <camera3d.h>

#define QTK_DEBUG

class MeshRenderer;
class Model;
class Object;
class Skybox;
class Texture;

class MainWidget : public QOpenGLWidget,
                   protected QOpenGLFunctions {
Q_OBJECT;

public:
  // Constructors
  MainWidget();
  explicit MainWidget(const QSurfaceFormat &format);
  ~MainWidget() override;

  // Static Members
  static inline QMatrix4x4 &Projection() { return mProjection;}
  static inline Camera3D &Camera() { return mCamera;}

private:
  void teardownGL();
  void initObjects();

public:
  // Inherited virtual Members
  void paintGL() override;
  void initializeGL() override;
  void resizeGL(int width, int height) override;

protected slots:
  void update();
  void messageLogged(const QOpenGLDebugMessage &msg);

  // Protected Helpers
protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  // Private helpers
  void printContextInformation();
  void updateCameraInput();

  // Private Members
  static Camera3D mCamera;
  static QMatrix4x4 mProjection;

  Skybox * mSkybox;
  Object * mObject;
  MeshRenderer * mTestPhong;
  MeshRenderer * mTestSpecular;
  MeshRenderer * mTestDiffuse;
  MeshRenderer * mTestAmbient;
  std::vector<MeshRenderer *> mMeshes;
  std::vector<Model *> mModels;

  QOpenGLDebugLogger * mDebugLogger;
};

#endif // QTK_MAINWIDGET_H
