/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Collection of widget plugins for Qt Designer                        ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#ifndef QTK_WIDGETPLUGINCOLLECTION_H
#define QTK_WIDGETPLUGINCOLLECTION_H

#include <QDesignerCustomWidgetCollectionInterface>

class WidgetPluginCollection :
    public QObject,
    public QDesignerCustomWidgetCollectionInterface {
    Q_OBJECT
    // Since we're exporting a collection, this is the only plugin metadata
    // needed. We don't need this for-each widget in the collection.
    Q_PLUGIN_METADATA(IID "com.Klips.WidgetPluginCollection")
    // Tell Qt Object system that we're implementing an interface.
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

  public:
    /***************************************************************************
     * Contructors / Destructors
     **************************************************************************/

    explicit WidgetPluginCollection(QObject * parent = nullptr);

    /***************************************************************************
     * Public Methods
     **************************************************************************/

    /**
     * @return QList of all custom widgets pointers.
     */
    [[nodiscard]] QList<QDesignerCustomWidgetInterface *> customWidgets() const;

  private:
    /***************************************************************************
     * Private Members
     **************************************************************************/

    QList<QDesignerCustomWidgetInterface *> m_collection;
    QString m_collectionName;
};

#endif  // QTK_WIDGETPLUGINCOLLECTION_H
