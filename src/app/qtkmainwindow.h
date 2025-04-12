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

#include <QFileDialog>
#include <QMainWindow>

#include "designer-plugins/debugconsole.h"

namespace Ui
{
  class MainWindow;
}

/**
 * An empty scene used for initializing all QtkWidgets within the MainWindow.
 * This serves as a temporary placeholder for QtkScene (for example), which is
 * defined in the separate qtk_gui target. The reason for this separation is to
 * support the use of QtkWidgets (the qtk_plugins target) within the Qt Designer
 * application without implementations provided in the Qtk Desktop Application.
 *
 * For the Qtk application, this should be replaced by loading the previous
 * scene or creating a new _unsaved_ scene when the application is opened.
 * Currently we have essentially hard-coded QtkScene to use as examples for
 * testing the application. This means that the only way to create or modify a
 * scene is to write code. Any modifications made in the application, such as
 * moving or resizing objects, will not persist and cannot be saved.
 *
 * For users of Qtk Designer Plugins, this means that installing
 * the `qtk_plugins` target to Qt Designer allows use all of the designer's
 * features to build an interface and position or resize a QtkWidget as needed.
 * The QtkWidget also appears as widget in the IDE's toolbars and can be added
 * to any new application easily, once the plugins are installed.
 *
 * Once the application is designed, you can define a custom scene and use the
 * Qtk API or Qt OpenGL funtions directly to render to it.
 *
 * Any application using a QtkWidget can set a custom scene in their main
 * function. See the MainWindow::MainWindow constructor as an example.
 */
class EmptyScene : public Qtk::Scene
{
    void init() override
    {
      setSkybox(new Qtk::Skybox(":/textures/skybox/right.png",
                                ":/textures/skybox/top.png",
                                ":/textures/skybox/front.png",
                                ":/textures/skybox/left.png",
                                ":/textures/skybox/bottom.png",
                                ":/textures/skybox/back.png",
                                "Skybox"));
      setSceneName("Empty Scene");
    }
};

/*
 * Conditionally include the QtkScene header if the example is enabled.
 * Set AppScene type to use in main() for creating the scene.
 * Define helper function to initialize Qt resources for the application.
 *    These resources are different based on if the example is enabled.
 */
#ifdef QTK_GUI_SCENE
#include "qtkscene.h"
using AppScene = QtkScene;
inline void initResources()
{
  Q_INIT_RESOURCE(resources);
}
#else
using AppScene = EmptyScene;
inline void initResources()
{
  Q_INIT_RESOURCE(minimal_resources);
}
#endif

/**
 * MainWindow class to provide an example of using a QtkWidget within a Qt
 * window application.
 */
class MainWindow : public QMainWindow
{
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

    /**
     * @param scene The new scene to view.
     */
    void setScene(Qtk::Scene * scene);

    /**
     * @return Default icon to use for Qtk desktop application.
     */
    static QIcon getIcon() { return QIcon(":/icons/icon.png"); }

  public slots:
    /**
     * Trigger a refresh for widgets related to a scene that has been updated.
     * @param sceneName The name of the scene that has been modified.
     */
    void refreshScene(const QString & sceneName);


    /**
     * Opens a QFileDialog for selecting an object file to load into the scene.
     */
    void loadObject()
    {
      const QUrl file = QFileDialog::getOpenFileName(
          this, tr("Load Model"), QDir::homePath(), tr("Object Files (*.obj)"));
      getQtkWidget()->getScene()->loadModel(file.fileName().replace(".obj", ""),
                                            file.toString());
    }

    /**
     * Deletes the currently selected object from the scene.
     */
    void deleteObject();

  private:
    /***************************************************************************
     * Private Members
     **************************************************************************/

    /** Do not allow copying */
    MainWindow(const MainWindow &) {};

    Ui::MainWindow * ui_ {};

    /**
     * Maps a scene name to the QtkWidget viewing it.
     * TODO: Value should be a vector of QtkWidget * for multiple scene views.
     */
    std::unordered_map<QString, Qtk::QtkWidget *> views_ {};
};

#endif  // MAINWINDOW_H
