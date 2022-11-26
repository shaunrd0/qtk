/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: MainWindow for creating an example Qt application                   ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <mainwindow.h>
#include <qtkwidget.h>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
  // For use in design mode using Qt Creator
  // + We can use the `ui` member to access nested widgets by name
  ui->setupUi(this);

  // Find all QtkWidgets in this QMainWindow and initialize their scenes.
  for(const auto widget : ui->qWidget->children()) {
    auto qtkWidget = dynamic_cast<Qtk::QtkWidget *>(widget);
    if(qtkWidget != nullptr) {
      std::string key = qtkWidget->objectName().toStdString();
      // Initialize each scene into a map if it doesn't exist.
      if(mScenes[key] == nullptr) {
        mScenes[key] = new ExampleScene();
      }
      // Set the QtkWidget to use the scene associated with this widget.
      qtkWidget->setScene(mScenes[key]);
    }
  }

  // Set the window icon used for Qtk.
  // TODO: Update this to be something other than kilroy.
  setWindowIcon(QIcon("../resources/icon.png"));
}

MainWindow::~MainWindow() {
  delete ui;
}
