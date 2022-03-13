#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "main-widget_export.h"

namespace Ui {
  class MainWindow;
}

class MAIN_WIDGET_EXPORT MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
