/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Main window for Qt6 OpenGL widget application                       ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_QTKWIDGET_H
#define QTK_QTKWIDGET_H

#include <iostream>

#include <QMatrix4x4>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <qtkapi.h>
#include <abstractscene.h>

namespace Qtk {
  class QTKAPI QtkWidget : public QOpenGLWidget,
                           protected QOpenGLFunctions {
  Q_OBJECT;

  public:
    // Constructors
    QtkWidget();
    explicit QtkWidget(QWidget *parent);
    explicit QtkWidget(const QSurfaceFormat &format);
    ~QtkWidget() override;

  private:
    void teardownGL();

  public:
    // Inherited virtual Members
    void paintGL() override;
    void initializeGL() override;
    void resizeGL(int width, int height) override;

    inline Qtk::Scene * getScene() {return mScene;}
    inline void setScene(Qtk::Scene * scene) {
      if (mScene != Q_NULLPTR) delete mScene;
      mScene = scene;
    }

  protected slots:
    void update();
#ifdef QTK_DEBUG
    void messageLogged(const QOpenGLDebugMessage &msg);
#endif

    // Protected Helpers
  protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

  private:
    // Private helpers
    void initializeWidget();
    void updateCameraInput();

    Qtk::Scene * mScene;
#ifdef QTK_DEBUG
    void printContextInformation();
    QOpenGLDebugLogger * mDebugLogger;
#endif
  };
}

#endif // QTK_QTKWIDGET_H
