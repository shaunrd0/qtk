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

ToolBox::ToolBox(QWidget * parent) :
    QDockWidget(parent), objectDetails_(this), transformPanel_(this),
    scalePanel_(this), vertex_(this, "Vertex Shader:"),
    fragment_(this, "Fragment Shader:"), properiesForm_(new QFormLayout),
    shaderForm_(new QFormLayout), ui(new Ui::ToolBox)
{
  ui->setupUi(this);
  setMinimumWidth(350);

  // Object Properties.
  ui->page_properties->setLayout(properiesForm_);
  properiesForm_->addRow(objectDetails_.name.label, objectDetails_.name.value);
  properiesForm_->addRow(objectDetails_.objectType.label,
                         objectDetails_.objectType.value);
  properiesForm_->addRow(reinterpret_cast<QWidget *>(&transformPanel_));
  properiesForm_->addRow(reinterpret_cast<QWidget *>(&scalePanel_));
  ui->toolBox->setCurrentWidget(ui->page_properties);

  // Shader views.
  ui->page_shaders->setLayout(shaderForm_);
  shaderForm_->addRow(reinterpret_cast<QWidget *>(&vertex_));
  shaderForm_->addRow(reinterpret_cast<QWidget *>(&fragment_));
}

void ToolBox::updateFocus(const QString & name)
{
  auto object =
      QtkWidget::mWidgetManager.get_widget()->getScene()->getObject(name);
  if (object != Q_NULLPTR) {
    refreshProperties(object);
    refreshShaders(object);
  }
}

ToolBox::SpinBox3D::SpinBox3D(QWidget * parent, const char * l) :
    QWidget(parent), layout(new QHBoxLayout(this)), label(new QLabel(tr(l)))
{
  // The layout owns the widget and will clean it up on destruction.
  layout->addWidget(label);
  for (const auto & f : fields) {
    layout->addWidget(f->spinBox);
    f->spinBox->setMinimum(std::numeric_limits<double>::lowest());
    f->spinBox->setSingleStep(0.1);
    f->spinBox->setFixedWidth(75);
  }
}

void ToolBox::SpinBox::disconnect() const
{
  Object::disconnect(connection);
}

void ToolBox::TransformPanel::setObject(const Qtk::Object * object)
{
  // Reconnect translation panel controls to the new object.
  const std::vector binds = {&Object::setTranslationX,
                             &Object::setTranslationY,
                             &Object::setTranslationZ};
  for (size_t i = 0; i < spinBox3D.fields.size(); i++) {
    auto * f = spinBox3D.fields[i];
    // Disconnect before changing spin box value.
    f->disconnect();

    // Set the values in the spin box to the object's current X,Y,Z
    f->spinBox->setValue(object->getTransform().getTranslation()[i]);

    // Reconnect to bind spin box value to the new object's position.
    f->connection =
        connect(f->spinBox, &QDoubleSpinBox::valueChanged, object, binds[i]);
  }
}

void ToolBox::ScalePanel::setObject(const Qtk::Object * object)
{
  // Reconnect scale panel controls to the new object.
  const std::vector binds = {
      &Object::setScaleX, &Object::setScaleY, &Object::setScaleZ};
  for (size_t i = 0; i < spinBox3D.fields.size(); i++) {
    auto * f = spinBox3D.fields[i];
    // Disconnect before changing spin box value.
    f->disconnect();

    // Set the values in the spin box to the object's current X,Y,Z
    f->spinBox->setValue(object->getTransform().getScale()[i]);

    // Reconnect to bind spin box value to the new object's scale.
    f->connection =
        connect(f->spinBox, &QDoubleSpinBox::valueChanged, object, binds[i]);
  }
}

ToolBox::~ToolBox()
{
  delete ui;
}

void ToolBox::refreshProperties(const Object * object)
{
  // Refresh to show the new object's details.
  objectDetails_.setObject(object);
  // Reconnect transform panel controls to the new object.
  transformPanel_.setObject(object);
  scalePanel_.setObject(object);
}

void ToolBox::refreshShaders(const Object * object)
{
  vertex_.path.setValue(object->getVertexShader().c_str());
  vertex_.editor->setText(object->getVertexShaderSourceCode().c_str());
  fragment_.path.setValue(object->getFragmentShader().c_str());
  fragment_.editor->setText(object->getFragmentShaderSourceCode().c_str());
}
