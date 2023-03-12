/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Collection of widget plugins for Qt Designer                        ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#include "widgetplugincollection.h"
#include "debugconsole.h"
#include "qtkwidget.h"
#include "toolbox.h"
#include "treeview.h"
#include "widgetplugin.h"

/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

WidgetPluginCollection::WidgetPluginCollection(QObject * parent) :
    QObject(parent), m_collectionName("Qtk Widget Collection") {
  m_collection = {
      new WidgetPlugin(
          m_collectionName, "Qtk::QtkWidget", "qtkwidget.h",
          [](QWidget * parent) { return new Qtk::QtkWidget(parent); }),
      new WidgetPlugin(
          m_collectionName, "Qtk::TreeView", "treeview.h",
          [](QWidget * parent) { return new Qtk::TreeView(parent); }),
      new WidgetPlugin(
          m_collectionName, "Qtk::ToolBox", "toolbox.h",
          [](QWidget * parent) { return new Qtk::ToolBox(parent); }),
  };
}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

QList<QDesignerCustomWidgetInterface *> WidgetPluginCollection::customWidgets()
    const {
  return m_collection;
}
