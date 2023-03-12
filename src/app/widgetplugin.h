/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Generic Qt Designer widget plugin                                   ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#ifndef QTK_WIDGETPLUGIN_H
#define QTK_WIDGETPLUGIN_H

#include <QDesignerCustomWidgetInterface>
#include <QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT WidgetPlugin :
    public QObject,
    public QDesignerCustomWidgetInterface {
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

    using Factory = std::function<QWidget *(QWidget *)>;

  public:
    /***************************************************************************
     * Contructors / Destructors
     **************************************************************************/

    WidgetPlugin(
        QString group, QString class_name, QString include, Factory factory);

    explicit WidgetPlugin(QObject * parent = nullptr);

    ~WidgetPlugin() = default;

    /***************************************************************************
     * Public Methods
     **************************************************************************/

    /**
     * @return The name of the group to which this widget belongs.
     */
    [[nodiscard]] QString group() const override;

    /**
     * Must return the _class name_ of the widget.
     *
     * @return The class name for the associated widget.
     */
    [[nodiscard]] QString name() const override;

    /**
     * If this path changes for a custom widget, it must be removed and added
     * back in Qt Designer for the XML surrounding this value to be regenerated.
     *
     * See the `<customwidget>` XML in any `.ui` file using a custom widget.
     *
     * @return Path to the include file for UIC to use when generating code.
     */
    [[nodiscard]] QString includeFile() const override;

    /**
     * @param parent Parent widget to the new instance of this widget.
     * @return A new instance of this custom widget.
     */
    [[nodiscard]] QWidget * createWidget(QWidget * parent) override;

    /**
     * @return Short description used in Qt Designer tool tips.
     */
    [[nodiscard]] QString toolTip() const override;

    /**
     * @return Widget description used in `What's this?` within Qt Creator.
     */
    [[nodiscard]] QString whatsThis() const override;

    /**
     * @return Icon used to represent the widget in Qt Designer's GUI.
     */
    [[nodiscard]] QIcon icon() const override;

    /**
     * Whether or not this widget should act as a container for other widgets.
     *
     * @return True if this custom widget is meant to be a container.
     */
    [[nodiscard]] bool isContainer() const override;

    /**
     * @return True if this widget has been initialized.
     */
    [[nodiscard]] bool isInitialized() const override;

    /**
     * Initializes an instance of this custom widget.
     * @param core
     */
    void initialize(QDesignerFormEditorInterface * core) override;

    /**
     * Default XML for an instance of this custom widget within a `.ui` file.
     *
     * Any property available for the widget in Qt Designer can be set using XML
     * properties, as seen here with `toolTip` and `whatsThis`.
     *
     * @return XML inserted for each instance of this widget.
     */
    [[nodiscard]] QString domXml() const override;

  private:
    /***************************************************************************
     * Private Members
     **************************************************************************/

    bool m_initialized = false;
    QString m_group;
    QString m_className;
    QString m_objectName;
    QString m_includeFile;
    Factory m_factory;
};

#endif  // QTK_WIDGETPLUGIN_H
