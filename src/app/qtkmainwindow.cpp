/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: MainWindow for Qtk application                                      ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "qtkmainwindow.h"
#include "ui_qtkmainwindow.h"

/*******************************************************************************
 * Constructors / Destructors
 ******************************************************************************/

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent)
{
  ui_ = new Ui::MainWindow;
  setObjectName("MainWindow");
  // For use in design mode using Qt Creator
  // + We can use the `ui` member to access nested widgets by name
  ui_->setupUi(this);
  ui_->menuView->addAction(ui_->toolBar->toggleViewAction());

  // Initialize static container for all active QtkWidgets
  auto qtkWidgets = findChildren<Qtk::QtkWidget *>();
  for (auto & qtkWidget : qtkWidgets) {
    qtkWidget->setMainWindow(this);
    // NOTE: Set a temporary scene for the widget to use for initialization.
    // This should be replaced by loading a scene, or creating a new (unsaved)
    // scene when Qtk is opened.
    qtkWidget->setScene(new EmptyScene);
    views_.emplace(qtkWidget->getScene()->getSceneName(), qtkWidget);

    // Add GUI 'view' toolbar option to show debug console.
    ui_->menuView->addAction(qtkWidget->getActionToggleConsole());

    // Refresh GUI widgets when scene or objects are updated.
    connect(qtkWidget->getScene(),
            &Qtk::Scene::sceneUpdated,
            this,
            &MainWindow::refreshScene);

    // Update the ToolBox details panel when an item is double-clicked.
    connect(qtkWidget,
            &Qtk::QtkWidget::objectFocusChanged,
            ui_->qtk__ToolBox,
            &Qtk::ToolBox::updateFocus);
  }

  connect(ui_->actionDelete_Object,
          &QAction::triggered,
          this,
          &MainWindow::deleteObject);

  connect(ui_->actionLoad_Model,
          &QAction::triggered,
          this,
          &MainWindow::loadObject);

  // TODO: Fix / use MainWindow in Qt Designer to add these dock widgets.
  // For now we will add them manually, but we should be able to do this in the
  // designer. At the moment if you edit the UI in designer the dock widget
  // areas below will override the designer settings.

  // Dock the toolbox widget to the main window.
  addDockWidget(Qt::LeftDockWidgetArea, ui_->qtk__ToolBox);
  // Add an option to toggle active widgets in the GUI's toolbar 'view' menu.
  ui_->menuView->addAction(ui_->qtk__ToolBox->toggleViewAction());

  addDockWidget(Qt::RightDockWidgetArea, ui_->qtk__TreeView);
  ui_->menuView->addAction(ui_->qtk__TreeView->toggleViewAction());

  // Set the window icon used for Qtk.
  setWindowIcon(getIcon());
}

MainWindow::~MainWindow()
{
  delete ui_;
}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

MainWindow * MainWindow::getMainWindow()
{
  static auto * window = new MainWindow;
  return window;
}

Qtk::QtkWidget * MainWindow::getQtkWidget(int64_t index)
{
  if (views_.size() <= index) {
    return Q_NULLPTR;
  }
  auto it = views_.begin();
  std::advance(it, index);
  return it->second;
}

Qtk::QtkWidget * MainWindow::getQtkWidget(const QString & name)
{
  if (!views_.count(name)) {
    return Q_NULLPTR;
  }
  return views_[name];
}

void MainWindow::refreshScene(const QString & sceneName)
{
  // TODO: Select TreeView using sceneName
  ui_->qtk__TreeView->updateView(getQtkWidget()->getScene());
}

void MainWindow::deleteObject()
{
  if (auto object = ui_->qtk__ToolBox->getObjectFocus(); object != Q_NULLPTR) {
    auto scene = getQtkWidget()->getScene();
    switch (object->getType()) {
      case Qtk::Object::Type::QTK_MESH:
        scene->removeObject(dynamic_cast<Qtk::MeshRenderer *>(object));
        ui_->qtk__ToolBox->clearFocus();
        break;
      case Qtk::Object::Type::QTK_MODEL:
        scene->removeObject(dynamic_cast<Qtk::Model *>(object));
        ui_->qtk__ToolBox->clearFocus();
        break;
      default:
        qDebug() << "Failed to delete model with invalid type";
        break;
    }
  }
}

void MainWindow::setScene(Qtk::Scene * scene)
{
  connect(scene,
          &Qtk::Scene::sceneUpdated,
          MainWindow::getMainWindow(),
          &MainWindow::refreshScene);
  ui_->qtk__QtkWidget->setScene(scene);
}
