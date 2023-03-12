/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: TreeView plugin for scene hierarchy                                 ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#include "treeview.h"
#include "qtkmainwindow.h"
#include "ui_treeview.h"

/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

Qtk::TreeView::TreeView(QWidget * parent) :
    QDockWidget(parent), ui(new Ui::TreeView) {
  ui->setupUi(this);
  connect(
      ui->treeWidget, &QTreeWidget::itemDoubleClicked, this,
      &TreeView::itemFocus);
}

Qtk::TreeView::~TreeView() {
  delete ui;
}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

void Qtk::TreeView::updateView(const Qtk::Scene * scene) {
  ui->treeWidget->clear();
  ui->treeWidget->setColumnCount(1);
  mSceneName = scene->getSceneName();
  auto objects = scene->getObjects();
  for(const auto & object : objects) {
    auto item =
        new QTreeWidgetItem(QStringList(QString(object->getName().c_str())));
    ui->treeWidget->insertTopLevelItem(0, item);
  }
}

void Qtk::TreeView::itemFocus(QTreeWidgetItem * item, int column) {
  QString name = item->text(column);
  auto scene = MainWindow::getMainWindow()->getQtkWidget()->getScene();
  auto & transform = scene->getCamera().getTransform();
  auto object = scene->getObject(name);
  if(object == Q_NULLPTR) {
    qDebug() << "Attempt to get non-existing object with name '" << name
             << "'\n";
  }
  Transform3D * objectTransform;
  if(object->getType() == Object::QTK_MESH) {
    objectTransform = &dynamic_cast<MeshRenderer *>(object)->getTransform();
  } else if(object->getType() == Object::QTK_MODEL) {
    objectTransform = &dynamic_cast<Model *>(object)->getTransform();
  }
  transform.setTranslation(objectTransform->getTranslation());
  transform.translate(0.0f, 0.0f, 3.0f);
}
