/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: QtkWidget for Qt desktop application                                ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_QTKWIDGET_H
#define QTK_QTKWIDGET_H

#include <iostream>

#include <QDockWidget>
#include <QMatrix4x4>
#include <QOpenGLDebugLogger>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPlainTextEdit>

#include <qtk/qtkapi.h>
#include <qtk/scene.h>

namespace Qtk {
  class DebugConsole;

  /**
   * QtkWidget class to define required QOpenGLWidget functionality.
   *
   * This object has a Scene attached which manages the objects to render.
   * Client input is passed through this widget to control the camera view.
   */
  class QtkWidget : public QOpenGLWidget, protected QOpenGLFunctions {
      Q_OBJECT;

    public:
      /*************************************************************************
       * Contructors / Destructors
       ************************************************************************/

      /**
       * Qt Designer will call this ctor when creating this widget as a child.
       *
       * @param parent Pointer to a parent widget for this QtkWidget or nullptr.
       */
      explicit QtkWidget(QWidget * parent = nullptr);

      /**
       * Default construct a QtkWidget.
       *
       * @param parent Pointer to a parent widget or nullptr if no parent.
       * @param name An objectName for the new QtkWidget.
       */
      explicit QtkWidget(QWidget * parent, const QString & name);

      /**
       * Construct a custom QtkWidget.
       *
       * @param parent Pointer to a parent widget or nullptr if no parent.
       * @param name An objectName for the new QtkWidget.
       * @param scene Pointer to a custom class inheriting from Qtk::Scene.
       */
      QtkWidget(QWidget * parent, const QString & name, Qtk::Scene * scene);

      ~QtkWidget();

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      /**
       * Constructs a QAction to hide / show this DebugConsole.
       * @return QAction to toggle visibility of this DebugConsole.
       */
      QAction * getActionToggleConsole();

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

      /**
       * @return The active scene being viewed in this widget.
       */
      inline Qtk::Scene * getScene() { return mScene; }

      /**
       * @return Pointer to the QOpenGLDebugLogger attached to this widget.
       */
      inline QOpenGLDebugLogger * getOpenGLDebugLogger() {
        return mDebugLogger;
      }

      /*************************************************************************
       * Setters
       ************************************************************************/

      /**
       * @param scene The new scene to view.
       */
      void setScene(Qtk::Scene * scene);

    public slots:

      /**
       * Toggle visibility of the DebugConsole associated with this QtkWidget.
       */
      void toggleConsole();

    signals:
      /**
       * Log a message to the DebugConsole associated with this widget.
       * @param message The message to log.
       * @param context The context of the log message.
       */
      void sendLog(const QString & message, DebugContext context = Status);

    protected:
      /*************************************************************************
       * Protected Methods
       ************************************************************************/

      /**
       * @param event Key press event to update camera input manager.
       */
      void keyPressEvent(QKeyEvent * event) override;

      /**
       * @param event Key release event to update camera input manager.
       */
      void keyReleaseEvent(QKeyEvent * event) override;

      /**
       * @param event Mouse button press event to update camera input manager.
       */
      void mousePressEvent(QMouseEvent * event) override;

      /**
       * @param event Mouse button release event to update camera input manager.
       */
      void mouseReleaseEvent(QMouseEvent * event) override;

    protected slots:
      /**
       * Called when the `frameSwapped` signal is caught.
       * See definition of initializeGL()
       */
      void update();

      /**
       * Called when the `messageLogged` signal is caught.
       * See definition of initializeGL()
       *
       * @param msg The message logged.
       */
      void messageLogged(const QOpenGLDebugMessage & msg);

    private:
      /*************************************************************************
       * Private Methods
       ************************************************************************/

      /**
       * Deconstruct any resources we have allocated for this widget.
       */
      void teardownGL();

      /**
       * Callback function to update input for camera controls
       */
      static void updateCameraInput();

      /**
       * Prints OpenGL context information at start of debug session.
       */
      void printContextInformation();

      /*************************************************************************
       * Private Members
       ************************************************************************/

      QOpenGLDebugLogger * mDebugLogger;
      Qtk::Scene * mScene;
      Qtk::DebugConsole * mConsole;
      bool mConsoleActive = false;
  };
}  // namespace Qtk

#endif  // QTK_QTKWIDGET_H
