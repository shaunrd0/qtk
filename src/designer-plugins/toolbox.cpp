/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Toolbox plugin for object details and options                       ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#include "toolbox.h"
#include "qtkwidget.h"
#include "ui_toolbox.h"

#include <QFormLayout>
#include <QLabel>

using namespace Qtk;

ToolBox::ToolBox(QWidget * parent) : QDockWidget(parent), ui(new Ui::ToolBox)
{
  ui->setupUi(this);
  setMinimumWidth(350);
}

void ToolBox::updateFocus(const QString & name)
{
  auto object =
      Qtk::QtkWidget::mWidgetManager.get_widget()->getScene()->getObject(name);
  if (object != Q_NULLPTR) {
    removePages();
    createPageProperties(object);
    createPageShader(object);
  }
}

ToolBox::~ToolBox()
{
  delete ui;
}

void ToolBox::removePages()
{
  // Remove all existing pages.
  for (size_t i = 0; i < ui->toolBox->count(); i++) {
    delete ui->toolBox->widget(i);
    ui->toolBox->removeItem(i);
  }
}

void ToolBox::createPageProperties(const Object * object)
{
  auto transform = object->getTransform();
  auto type = object->getType();
  auto * widget = new QWidget;
  ui->toolBox->addItem(widget, "Properties");
  ui->toolBox->setCurrentWidget(widget);

  auto * layout = new QFormLayout;
  layout->addRow(new QLabel(tr("Name:")),
                 new QLabel(object->getName().c_str()));

  layout->addRow(new QLabel(tr("Type:")),
                 new QLabel(type == Object::Type::QTK_MESH ? "Mesh" : "Model"));

  auto rowLayout = new QHBoxLayout;
  rowLayout->addWidget(new QLabel(tr("Translation:")));
  int minWidth = 75;
  for (size_t i = 0; i < 3; i++) {
    auto spinBox = new QDoubleSpinBox;
    spinBox->setMinimum(std::numeric_limits<double>::lowest());
    spinBox->setSingleStep(0.1);
    spinBox->setValue(transform.getTranslation()[i]);
    spinBox->setFixedWidth(minWidth);
    rowLayout->addWidget(spinBox);

    if (i == 0) {
      connect(spinBox,
              &QDoubleSpinBox::valueChanged,
              object,
              &Object::setTranslationX);
    } else if (i == 1) {
      connect(spinBox,
              &QDoubleSpinBox::valueChanged,
              object,
              &Object::setTranslationY);
    } else if (i == 2) {
      connect(spinBox,
              &QDoubleSpinBox::valueChanged,
              object,
              &Object::setTranslationZ);
    }
  }
  layout->addRow(rowLayout);

  rowLayout = new QHBoxLayout;
  rowLayout->addWidget(new QLabel(tr("Scale:")));
  for (size_t i = 0; i < 3; i++) {
    auto spinBox = new QDoubleSpinBox;
    spinBox->setMinimum(std::numeric_limits<double>::lowest());
    spinBox->setSingleStep(0.1);
    spinBox->setValue(transform.getScale()[i]);
    spinBox->setFixedWidth(minWidth);
    rowLayout->addWidget(spinBox);

    if (i == 0) {
      connect(
          spinBox, &QDoubleSpinBox::valueChanged, object, &Object::setScaleX);
    } else if (i == 1) {
      connect(
          spinBox, &QDoubleSpinBox::valueChanged, object, &Object::setScaleY);
    } else if (i == 2) {
      connect(
          spinBox, &QDoubleSpinBox::valueChanged, object, &Object::setScaleZ);
    }
  }
  layout->addRow(rowLayout);
  widget->setLayout(layout);
}

void ToolBox::createPageShader(const Object * object)
{
  // Shaders page.
  auto widget = new QWidget;
  ui->toolBox->addItem(widget, "Shaders");
  auto mainLayout = new QFormLayout;
  auto rowLayout = new QHBoxLayout;
  rowLayout->addWidget(new QLabel("Vertex Shader:"));
  rowLayout->addWidget(new QLabel(object->getVertexShader().c_str()));
  mainLayout->addRow(rowLayout);

  auto shaderView = new QTextEdit;
  shaderView->setReadOnly(true);
  shaderView->setText(object->getVertexShaderSourceCode().c_str());
  mainLayout->addRow(shaderView);

  rowLayout = new QHBoxLayout;
  rowLayout->addWidget(new QLabel("Fragment Shader:"));
  rowLayout->addWidget(new QLabel(object->getFragmentShader().c_str()));
  mainLayout->addRow(rowLayout);

  shaderView = new QTextEdit;
  shaderView->setReadOnly(true);
  shaderView->setText(object->getFragmentShaderSourceCode().c_str());
  mainLayout->addRow(shaderView);

  widget->setLayout(mainLayout);
}
