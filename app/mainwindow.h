/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: MainWindow for creating an example Qt application                   ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unordered_map>

#include <QMainWindow>

#include <examplescene.h>
#include "qtk-widget_export.h"

namespace Ui {
  class MainWindow;
}

/**
 * MainWindow class to provide an example of using a QtkWidget within a Qt
 * window application.
 */
class QTK_WIDGET_EXPORT MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    /***************************************************************************
     * Contructors / Destructors
     **************************************************************************/

    /**
     * This ctor also initializes the Scene for each QtkWidget in the window.
     * To load a different scene this would need to be updated.
     *
     * @param parent The parent for this QMainWindow
     */
    explicit MainWindow(QWidget * parent = nullptr);
    ~MainWindow() override;

  private:
    /***************************************************************************
     * Private Members
     **************************************************************************/

    Ui::MainWindow * ui {};
    std::unordered_map<std::string, Qtk::Scene *> mScenes {};
};

#endif  // MAINWINDOW_H
