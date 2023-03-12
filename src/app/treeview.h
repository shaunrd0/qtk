/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: TreeView plugin for scene hierarchy                                 ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QDesignerCustomWidgetInterface>
#include <QDesignerExportWidget>
#include <QDockWidget>

#include <qtk/scene.h>
#include <QTreeWidgetItem>

namespace Ui {
  class TreeView;
}

namespace Qtk {
  class TreeView : public QDockWidget {
      Q_OBJECT

    public:
      /*************************************************************************
       * Constructors / Destructors
       ************************************************************************/

      explicit TreeView(QWidget * parent = nullptr);

      ~TreeView();

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      /**
       * Updates the QTreeWidget with all objects within the scene.
       * @param scene The scene to load objects from.
       */
      void updateView(const Scene * scene);

    public slots:
      /**
       * Focus the camera on an item when it is double clicked.
       * Triggered by QTreeWidget::itemDoubleClicked signal.
       *
       * @param item The item that was double clicked
       * @param column The column of the item that was double clicked.
       *    This param is currently not used but required for this signal.
       */
      void itemFocus(QTreeWidgetItem * item, int column);

    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/

      Ui::TreeView * ui;

      /**
       * The name of the scene last loaded by this TreeWidget.
       * Used to load object data from a target scene.
       */
      QString mSceneName;
  };
}  // namespace Qtk

#endif  // TREEVIEW_H
