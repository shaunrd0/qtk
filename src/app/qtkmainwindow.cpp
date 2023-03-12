/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: MainWindow for Qtk application                                      ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "qtkmainwindow.h"
#include "qtkscene.h"
#include "ui_qtkmainwindow.h"

MainWindow * MainWindow::mainWindow_ = Q_NULLPTR;

/*******************************************************************************
 * Constructors / Destructors
 ******************************************************************************/

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent) {
  ui_ = new Ui::MainWindow;
  setObjectName("MainWindow");
  // For use in design mode using Qt Creator
  // + We can use the `ui` member to access nested widgets by name
  ui_->setupUi(this);
  ui_->menuView->addAction(ui_->toolBar->toggleViewAction());

  // Initialize static container for all active QtkWidgets
  auto qtkWidgets = findChildren<Qtk::QtkWidget *>();
  for(auto & qtkWidget : qtkWidgets) {
    qtkWidget->setScene(new Qtk::SceneEmpty);
    views_.emplace(qtkWidget->getScene()->getSceneName(), qtkWidget);
    ui_->menuView->addAction(qtkWidget->getActionToggleConsole());
    connect(
        qtkWidget->getScene(), &Qtk::Scene::sceneUpdated, this,
        &MainWindow::refreshScene);
  }

  auto docks = findChildren<QDockWidget *>();
  for(auto & dock : docks) {
    addDockWidget(Qt::RightDockWidgetArea, dock);
    ui_->menuView->addAction(dock->toggleViewAction());
  }

  // Set the window icon used for Qtk.
  setWindowIcon(Qtk::getIcon());
}

MainWindow::~MainWindow() {
  delete ui_;
}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

MainWindow * MainWindow::getMainWindow() {
  if(mainWindow_ == Q_NULLPTR) {
    mainWindow_ = new MainWindow;
  }
  return mainWindow_;
}

Qtk::QtkWidget * MainWindow::getQtkWidget(int64_t index) {
  if(views_.size() <= index) {
    return Q_NULLPTR;
  }
  return views_.begin(index)->second;
}

Qtk::QtkWidget * MainWindow::getQtkWidget(const QString & name) {
  if(!views_.count(name)) {
    return Q_NULLPTR;
  }
  return views_[name];
}

void MainWindow::refreshScene(QString sceneName) {
  // TODO: Select TreeView using sceneName>
  ui_->qtk__TreeView->updateView(getQtkWidget()->getScene());
}
