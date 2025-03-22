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
#include <QGroupBox>


#include "qtk/scene.h"

namespace Ui
{
  class ToolBox;
}

namespace Qtk
{
  class ToolBox : public QDockWidget
  {
      Q_OBJECT

    public:
      /*************************************************************************
       * Contructors / Destructors
       *************************************************************************/

      explicit ToolBox(QWidget * parent = nullptr);

      ~ToolBox();

      void removePages();

      void createPageProperties(const Object * object);

      void createPageShader(const Object * object);

      void updateFocus(const QString & name);


    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/

      Ui::ToolBox * ui;
  };
}  // namespace Qtk

#endif  // TOOLBOX_H
