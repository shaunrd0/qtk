/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Main program for practice using Qt6 widgets and OpenGL              ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QApplication>
#include <QLabel>

#include <mainwindow.h>
#include <qtkwidget.h>
#include <QSurfaceFormat>

int main(int argc, char * argv[]) {
  QApplication a(argc, argv);

  // Create window for Qt application using custom mainwindow.h
  MainWindow w;
  w.show();

  return QApplication::exec();
}
