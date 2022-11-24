/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Main window for Qt6 OpenGL widget application                       ##
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

#define QTK_DEBUG

class MeshRenderer;

class Model;

class Object;

class Scene;

class Skybox;

class OpenGLTextureFactory;

class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT;

  public:
    // Constructors
    MainWidget();
    explicit MainWidget(QWidget * parent);
    explicit MainWidget(const QSurfaceFormat & format);
    ~MainWidget() override;

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
    void messageLogged(const QOpenGLDebugMessage & msg);

    // Protected Helpers
  protected:
    void keyPressEvent(QKeyEvent * event) override;
    void keyReleaseEvent(QKeyEvent * event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;

  private:
    // Private helpers
    void initializeWidget();
    void printContextInformation();
    void updateCameraInput();

    Scene * mScene;
    Object * mObject;

    QOpenGLDebugLogger * mDebugLogger;
};

#endif  // QTK_MAINWIDGET_H
