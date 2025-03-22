/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Main window for Qt6 OpenGL widget application                       ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/
#ifndef QTK_QTKAPI_H
#define QTK_QTKAPI_H

#include <QFile>
#include <QWidget>
#include <QtCore/QtGlobal>

#ifdef QTK_SHARED
#if defined(QTK_EXPORT)
#define QTKAPI Q_DECL_EXPORT
#else
#define QTKAPI Q_DECL_IMPORT
#endif
#else
#define QTKAPI
#endif

namespace Qtk
{
  /**
   * Flag to set context for debug messages.
   */
  enum DebugContext { Status, Debug, Warn, Error, Fatal };

  /**
   * Find top level parent for a widget.
   *
   * @param widget Widget to start the search from.
   * @return Top level parent widget or Q_NULLPTR if no parent
   */
  static QWidget * topLevelParent(QWidget * widget)
  {
    QString name = widget->objectName();
    while (widget->parentWidget() != Q_NULLPTR) {
      widget = widget->parentWidget();
    }
    return widget;
  }
}  // namespace Qtk

#endif  // QTK_QTKAPI_H
