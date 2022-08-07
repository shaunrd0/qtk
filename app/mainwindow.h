#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <qtk-widget_export.h>
#include <scene.h>


namespace Ui {
  class MainWindow;
}

class QTK_WIDGET_EXPORT MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  Qtk::Scene * mScene;
};

#endif // MAINWINDOW_H
