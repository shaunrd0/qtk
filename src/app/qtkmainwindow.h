/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: MainWindow for Qtk application                                      ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unordered_map>

#include <QMainWindow>
#include <QPlainTextEdit>

#include "debugconsole.h"
#include "qtkwidget.h"

namespace Ui {
  class MainWindow;
}

/**
 * MainWindow class to provide an example of using a QtkWidget within a Qt
 * window application.
 */
class MainWindow : public QMainWindow {
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

    /***************************************************************************
     * Public Methods
     **************************************************************************/

    /**
     * Allows widgets to retrieve an instance of this root QMainWindow.
     * @return this
     */
    static MainWindow * getMainWindow();

    Qtk::QtkWidget * getQtkWidget(int64_t index = 0);

    /**
     * Accessor for retrieving a QtkWidget by it's objectName.
     * This function will not construct a new QtkWidget if none is found.
     *
     * @param name The objectName associated with the QtkWidget.
     * @return Pointer to an active QtkWidget or Q_NULLPTR is not found.
     */
    Qtk::QtkWidget * getQtkWidget(const QString & name);

  public slots:
    /**
     * Trigger a refresh for widgets related to a scene that has been updated.
     * @param sceneName The name of the scene that has been modified.
     */
    void refreshScene(QString sceneName);

  private:
    /***************************************************************************
     * Private Members
     **************************************************************************/

    /** Do not allow copying */
    MainWindow(const MainWindow &) {};

    Ui::MainWindow * ui_ {};
    static MainWindow * mainWindow_;

    /**
     * Maps a scene name to the QtkWidget viewing it.
     * TODO: Value should be a vector of QtkWidget * for multiple scene views.
     */
    std::unordered_map<QString, Qtk::QtkWidget *> views_ {};
};

#endif  // MAINWINDOW_H
