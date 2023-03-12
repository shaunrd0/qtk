/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Example Qt desktop application using Qtk                            ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QApplication>
#include <QMainWindow>

#include "examplewidget.h"

int main(int argc, char * argv[]) {
  QApplication app(argc, argv);

  auto window = new QMainWindow;
  window->setCentralWidget(new ExampleWidget);
  window->show();

  app.exec();
}