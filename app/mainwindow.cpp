#include <mainwindow.h>
#include <qtkwidget.h>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  // For use in design mode using Qt Creator
  // + We can use the `ui` member to access nested widgets by name
  for(const auto widget : ui->qWidget->children()) {
    auto qtkWidget = dynamic_cast<Qtk::QtkWidget *>(widget);
    if(qtkWidget != nullptr) {
      std::string key = qtkWidget->objectName().toStdString();
      if(mScenes[key] == nullptr) {
        mScenes[qtkWidget->objectName().toStdString()] = new ExampleScene();
      }
      qtkWidget->setScene(mScenes[qtkWidget->objectName().toStdString()]);
    }
  }
  setWindowIcon(QIcon("../resources/icon.png"));
}

MainWindow::~MainWindow() {
  delete ui;
}
