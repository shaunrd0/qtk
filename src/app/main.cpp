/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Main program for practice using Qt6 widgets and OpenGL              ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QApplication>

#include "qtkmainwindow.h"
#include "qtkscene.h"

int main(int argc, char * argv[]) {
  Q_INIT_RESOURCE(resources);

  QApplication a(argc, argv);

  auto window = MainWindow::getMainWindow();

  // Qtk currently uses the decorator pattern to save / load scenes.
  // This is a temporary solution and will be improved in the future.
  auto emptyScene = new Qtk::SceneEmpty;
  window->getQtkWidget()->setScene(new QtkScene(emptyScene));

  window->show();
  return QApplication::exec();
}
