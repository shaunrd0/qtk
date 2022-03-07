/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Main program for practice using Qt6 widgets and OpenGL              ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QApplication>
#include <QLabel>

#include <mainwidget.h>


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  // Set OpenGL Version information
  // Note: This format must be set before show() is called.
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4,5);
  // Set the number of samples used for glEnable(GL_MULTISAMPLING)
  format.setSamples(4);
  // Set the size of teh depth bufer for glEnable(GL_DEPTH_TEST)
  format.setDepthBufferSize(16);
#ifdef QTK_DEBUG
  format.setOption(QSurfaceFormat::DebugContext);
#endif // QTK_DEBUG

  // Set the widget up using a custom format
  MainWidget widget(format);

  return a.exec();
}

