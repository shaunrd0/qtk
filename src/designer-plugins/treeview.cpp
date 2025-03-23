/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: TreeView plugin for scene hierarchy                                 ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#include "treeview.h"
#include "qtkwidget.h"
#include "ui_treeview.h"

/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

Qtk::TreeView::TreeView(QWidget * parent) :
    QDockWidget(parent), ui(new Ui::TreeView)
{
  ui->setupUi(this);
  connect(
      ui->treeWidget, &QTreeWidget::itemClicked, this, &TreeView::itemSelect);
  connect(ui->treeWidget,
          &QTreeWidget::itemDoubleClicked,
          this,
          &TreeView::itemFocus);
}

Qtk::TreeView::~TreeView()
{
  delete ui;
}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

void Qtk::TreeView::updateView(const Qtk::Scene * scene)
{
  ui->treeWidget->clear();
  ui->treeWidget->setColumnCount(1);
  mSceneName = scene->getSceneName();
  auto objects = scene->getObjects();
  for (const auto & object : objects) {
    QStringList list(QStringList(QString(object->getName())));
    ui->treeWidget->insertTopLevelItem(0, new QTreeWidgetItem(list));
  }
}

void Qtk::TreeView::itemFocus(QTreeWidgetItem * item, int column)
{
  const QString & name = item->text(column);
  auto scene = QtkWidget::mWidgetManager.get_widget()->getScene();
  auto object = scene->getObject(name);
  // If the object is a mesh or model, focus the camera on it.
  if (object == Q_NULLPTR) {
    qDebug() << "Attempt to get non-existing object with name '" << name
             << "'\n";
    return;
  }
  const Transform3D & objectTransform = object->getTransform();

  auto & camera_transform = Qtk::Scene::getCamera().getTransform();
  auto focusScale = objectTransform.getScale();
  float width = focusScale.x() / 2.0f;
  float height = focusScale.y() / 2.0f;
  QVector3D pos = objectTransform.getTranslation();
  // pos.setX(pos.x() + width);
  pos.setY(pos.y() + height);
  camera_transform.setTranslation(pos);
  camera_transform.translate(0.0f, 0.0f, 3.0f);

  // Emit signal from qtk widget for new object focus. Triggers GUI updates.
  emit QtkWidget::mWidgetManager.get_widget()->objectFocusChanged(name);
}

void Qtk::TreeView::itemSelect(QTreeWidgetItem * item, int column)
{
  // Emit signal from qtk widget for new object focus. Triggers GUI updates.
  const QString & name = item->text(column);
  emit QtkWidget::mWidgetManager.get_widget()->objectFocusChanged(name);
}
