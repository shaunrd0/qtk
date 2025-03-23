/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Toolbox plugin for object details and options                       ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QDesignerExportWidget>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>


#include "qtk/scene.h"

namespace Ui
{
  class ToolBox;
}

namespace Qtk
{
  class ToolBox final : public QDockWidget
  {
      Q_OBJECT

    public:
      /*************************************************************************
       * Contructors / Destructors
       *************************************************************************/

      explicit ToolBox(QWidget * parent = nullptr);

      ~ToolBox();

      void refreshProperties(const Object * object);

      void refreshShaders(const Object * object);

      void updateFocus(const QString & name);


    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/

      /// Displays details on the object.
      struct ObjectDetails {
          /// Single item containing a label and value.
          struct Item {
              explicit Item(QWidget * parent,
                            const char * l = "Item:",
                            const char * v = "") :
                  label(new QLabel(tr(l), parent)),
                  value(new QLabel(tr(v), parent))
              {
              }

              void setValue(const char * v) { value->setText(tr(v)); }

              void setItem(const char * l, const char * v)
              {
                label->setText(tr(l));
                value->setText(tr(v));
              }

              QLabel * label;
              QLabel * value;
          };

          /// We pass the parent widget so that Qt handles releasing memory.
          explicit ObjectDetails(QWidget * parent) :
              name(parent, "Name:"), objectType(parent, "Object Type:")
          {
          }

          /// Refresh to display the new object's details
          void setObject(const Qtk::Object * object)
          {
            name.setItem("Name:", object->getName().toStdString().c_str());
            objectType.setItem(
                "Type:",
                object->getType() == Object::QTK_MESH ? "Mesh" : "Model");
          }

          Item name, objectType;
      };
      ObjectDetails objectDetails_;

      /// Structure to associate a QSpinBox with a connection.
      struct SpinBox {
          /**
           * Default constructor passes no parent to the QSpinBox.
           * It must be added to a layout for Qt to clean up the resources.
           */
          SpinBox() : spinBox(new QDoubleSpinBox) {}

          /// Disconnect the associated connection.
          void disconnect() const;

          QDoubleSpinBox * spinBox;
          QMetaObject::Connection connection;
      };

      /// Spinbox with 3 fields and a single label.
      class SpinBox3D final : QWidget
      {
        public:
          /// We pass a parent to ensure Qt will clean up resources.
          /// Assigning a QWidget to a QLayout also ensures Qt will clean up.
          explicit SpinBox3D(QWidget * parent, const char * l = "SpinBox3D:");

          /// The main layout for the SpinBox3D widget.
          QHBoxLayout * layout;

          /// Label for the SpinBox3D.
          QLabel * label;

          /// SpinBox and a connection for each field.
          SpinBox x, y, z;
          /// Array for iterating over fields.
          std::array<SpinBox *, 3> fields {&x, &y, &z};
      };

      /// Initialize the transform panel and configure QObject connections.
      struct TransformPanel {
          explicit TransformPanel(QWidget * parent) :
              spinBox3D(parent, "Transform:")
          {
          }

          /// Reconnect QObject connections and spin box values in UI.
          void setObject(const Qtk::Object * object);

          SpinBox3D spinBox3D;
      };
      TransformPanel transformPanel_;

      /// Initialize the scale panel and configure QObject connections.
      struct ScalePanel {
          explicit ScalePanel(QWidget * parent) : spinBox3D(parent, "Scale:") {}

          /// Reconnect QObject connections and spin box values in UI.
          void setObject(const Qtk::Object * object);

          SpinBox3D spinBox3D;
      };
      ScalePanel scalePanel_;

      /// Displays shader name, path, and read-only text view.
      class ShaderView final : QWidget
      {
        public:
          explicit ShaderView(QWidget * parent,
                              const char * l = "ShaderView:") :
              layout(new QVBoxLayout(this)), path(parent, l),
              editor(new QTextEdit(parent))
          {
            // Create a child horizontal layout for shader name and file path.
            auto * pathLayout = new QHBoxLayout;
            pathLayout->addWidget(path.label);
            pathLayout->addWidget(path.value);
            layout->addLayout(pathLayout);

            // Add the read-only text editor widget to the main layout.
            editor->setReadOnly(true);
            layout->addWidget(editor);
          }

          /// The main layout for the ShaderView widget.
          QVBoxLayout * layout;

          /// Shader name and path on disk.
          ObjectDetails::Item path;

          /// Read-only (for now) display of the shader source code.
          QTextEdit * editor;
      };
      ShaderView vertex_, fragment_;

      QFormLayout * properiesForm_;
      QFormLayout * shaderForm_;

      Ui::ToolBox * ui;
  };
}  // namespace Qtk

#endif  // TOOLBOX_H
