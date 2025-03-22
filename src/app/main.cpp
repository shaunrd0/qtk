/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Main program for practice using Qt6 widgets and OpenGL              ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QApplication>

#include "qtkmainwindow.h"

int main(int argc, char * argv[])
{
  initResources();
  QApplication a(argc, argv);

  auto window = MainWindow::getMainWindow();

  // Qtk currently uses the decorator pattern to save / load scenes.
  // This is a temporary solution and will be improved in the future.
  // NOTE: We set the scene here and not in QtkMainWindow to detach the scene
  // from the QtkWidget plugin (qtk_plugin_library build target).
  // Once we can save / load scenes, this call, and QtkScene, can be removed.
  window->setScene(new AppScene);

  window->show();

  return QApplication::exec();
}
