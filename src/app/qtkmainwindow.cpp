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

    // Add GUI 'view' toolbar option to show debug console.
    ui_->menuView->addAction(qtkWidget->getActionToggleConsole());
    // Refresh GUI widgets when scene or objects are updated.
    connect(
        qtkWidget->getScene(), &Qtk::Scene::sceneUpdated, this,
        &MainWindow::refreshScene);
    connect(
        qtkWidget, &Qtk::QtkWidget::objectFocusChanged, ui_->qtk__ToolBox,
        &Qtk::ToolBox::updateFocus);
  }

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

void MainWindow::refreshScene(const QString & sceneName) {
  // TODO: Select TreeView using sceneName
  ui_->qtk__TreeView->updateView(getQtkWidget()->getScene());
}
