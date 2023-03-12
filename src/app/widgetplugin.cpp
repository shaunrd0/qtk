/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Generic Qt Designer widget plugin                                   ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
################################################################################
*/

#include <QIcon>
#include <QtPlugin>
#include <utility>

#include <qtk/qtkapi.h>

#include "widgetplugin.h"

/*******************************************************************************
 * Constructors, Destructors
 ******************************************************************************/

WidgetPlugin::WidgetPlugin(
    QString group, QString class_name, QString include,
    WidgetPlugin::Factory factory) :
    m_group(std::move(group)),
    m_className(std::move(class_name)), m_includeFile(std::move(include)),
    m_factory(std::move(factory)), m_objectName(class_name) {}

WidgetPlugin::WidgetPlugin(QObject * parent) : QObject(parent) {}

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

QString WidgetPlugin::group() const {
  return m_group;
}

QString WidgetPlugin::name() const {
  return m_className;
}

QString WidgetPlugin::includeFile() const {
  return m_includeFile;
}

QWidget * WidgetPlugin::createWidget(QWidget * parent) {
  return m_factory(parent);
}

QString WidgetPlugin::toolTip() const {
  return QStringLiteral("A custom widget tool tip.");
}

QString WidgetPlugin::whatsThis() const {
  return QStringLiteral("Custom widget what's this?");
}

QIcon WidgetPlugin::icon() const {
  return Qtk::getIcon();
}

bool WidgetPlugin::isContainer() const {
  return true;
}

bool WidgetPlugin::isInitialized() const {
  return m_initialized;
}

void WidgetPlugin::initialize(QDesignerFormEditorInterface *) {
  if(m_initialized) {
    return;
  }

  m_initialized = true;
}
QString WidgetPlugin::domXml() const {
  return
      "<ui language=\"c++\">\n"
       " <widget class=\"" + m_className + "\" name=\"" + m_objectName + "\">\n"
       "  <property name=\"geometry\">\n"
       "   <rect>\n"
       "    <x>0</x>\n"
       "    <y>0</y>\n"
       "    <width>100</width>\n"
       "    <height>100</height>\n"
       "   </rect>\n"
       "  </property>\n"
       "  <property name=\"toolTip\" >\n"
       "   <string>" + toolTip() + "</string>\n"
       "  </property>\n"
       "  <property name=\"whatsThis\" >\n"
       "   <string>" + whatsThis() + "</string>\n"
       "  </property>\n"
       " </widget>\n"
       "</ui>\n";
}
