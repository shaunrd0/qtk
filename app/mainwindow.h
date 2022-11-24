#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unordered_map>

#include <QMainWindow>

#include <examplescene.h>
#include "qtk-widget_export.h"

namespace Ui {
  class MainWindow;
}

class QTK_WIDGET_EXPORT MainWindow

    : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget * parent = nullptr);
    ~MainWindow() override;

  private:
    Ui::MainWindow * ui {};
    std::unordered_map<std::string, Qtk::Scene *> mScenes {};
};

#endif  // MAINWINDOW_H
