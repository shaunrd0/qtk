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
#include <scene.h>

namespace Qtk {
  /**
   * QtkWidget class to define required QOpenGLWidget functionality.
   *
   * This object has a Scene attached which manages the objects to render.
   * Client input is passed through this widget to control the camera view.
   */
  class QTKAPI QtkWidget : public QOpenGLWidget, protected QOpenGLFunctions {
      Q_OBJECT;

    public:
      /*************************************************************************
       * Contructors / Destructors
       ************************************************************************/

      /**
       * Default ctor will configure a QSurfaceFormat with default settings.
       */
      QtkWidget();

      /**
       * Qt Designer will call this ctor when creating this widget as a child.
       *
       * @param parent The parent QWidget
       */
      explicit QtkWidget(QWidget * parent);

      /**
       * Allow constructing the widget with a preconfigured QSurfaceFormat.
       *
       * @param format QSurfaceFormat already configured by the caller.
       */
      explicit QtkWidget(const QSurfaceFormat & format);

      ~QtkWidget() override;

    private:
      /*************************************************************************
       * Private Methods
       ************************************************************************/

      // clang-format off
      void teardownGL() { /* Nothing to teardown yet... */ }
      // clang-format on

    public:
      /*************************************************************************
       * Public Inherited Virtual Methods
       ************************************************************************/

      /**
       * Called when the widget is first constructed.
       */
      void initializeGL() override;

      /**
       * Called when the application window is resized.
       *
       * @param width The new width of the window.
       * @param height The new height of the window.
       */
      void resizeGL(int width, int height) override;

      /**
       * Called when OpenGL repaints the widget.
       */
      void paintGL() override;

      /*************************************************************************
       * Accessors
       ************************************************************************/

      inline Qtk::Scene * getScene() { return mScene; }

      /*************************************************************************
       * Setters
       ************************************************************************/

      inline void setScene(Qtk::Scene * scene) {
        delete mScene;
        mScene = scene;
      }

    protected slots:
      /*************************************************************************
       * Qt Slots
       ************************************************************************/

      /**
       * Called when the `frameSwapped` signal is caught.
       * See definition of initializeGL()
       */
      void update();

#ifdef QTK_DEBUG
      /**
       * Called when the `messageLogged` signal is caught.
       * See definition of initializeGL()
       *
       * @param msg The message logged.
       */
      static void messageLogged(const QOpenGLDebugMessage & msg);
#endif

    protected:
      /*************************************************************************
       * Protected Methods
       ************************************************************************/

      void keyPressEvent(QKeyEvent * event) override;
      void keyReleaseEvent(QKeyEvent * event) override;
      void mousePressEvent(QMouseEvent * event) override;
      void mouseReleaseEvent(QMouseEvent * event) override;

    private:
      /*************************************************************************
       * Private Methods
       ************************************************************************/

      void initializeWidget();
      static void updateCameraInput();

#ifdef QTK_DEBUG
      void printContextInformation();
      QOpenGLDebugLogger * mDebugLogger;
#endif

      /*************************************************************************
       * Private Members
       ************************************************************************/

      Qtk::Scene * mScene;
  };
}  // namespace Qtk

#endif  // QTK_QTKWIDGET_H
