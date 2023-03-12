/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Debug console for qtk views                                         ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include <QMainWindow>
#include <QWindow>

#include "debugconsole.h"
#include "ui_debugconsole.h"

using namespace Qtk;

DebugConsole::DebugConsole(QWidget * owner, const QString & key) :
    DebugConsole(owner, key, key + "Debugger") {}

DebugConsole::DebugConsole(
    QWidget * owner, const QString & key, const QString & name) {
  ui_ = new Ui::DebugConsole;
  ui_->setupUi(this);
  setObjectName(name);
  mConsole = ui_->textEdit;
  setWidget(mConsole);
  setWindowTitle(name + " Debug Console");

  auto qtkWidget = dynamic_cast<QtkWidget *>(owner);
  if(qtkWidget) {
    connect(qtkWidget, &QtkWidget::sendLog, this, &DebugConsole::sendLog);
  }
}
