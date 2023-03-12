/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Toolbox plugin for object details and options                       ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#include "toolbox.h"
#include "ui_toolbox.h"

Qtk::ToolBox::ToolBox(QWidget * parent) :
    QDockWidget(parent), ui(new Ui::ToolBox) {
  ui->setupUi(this);
}

Qtk::ToolBox::~ToolBox() {
  delete ui;
}
