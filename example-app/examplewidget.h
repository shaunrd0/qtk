/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Example Qtk widget                                                  ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTKCLIENT_EXAMPLEWIDGET_H
#define QTKCLIENT_EXAMPLEWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "examplescene.h"

class ExampleWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT;

  public:
    explicit ExampleWidget(QWidget * parent = nullptr);

    ~ExampleWidget() = default;

    void initializeGL() override;

    void resizeGL(int width, int height) override;

    void paintGL() override;

  protected slots:
    void update();

  private:
    Qtk::Scene * mScene;
};

#endif  // QTKCLIENT_EXAMPLEWIDGET_H
