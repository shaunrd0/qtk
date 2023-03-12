/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Debug console for qtk views                                         ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_DEBUGCONSOLE_H
#define QTK_DEBUGCONSOLE_H

#include <QApplication>
#include <QDockWidget>
#include <QPlainTextEdit>
#include <QVBoxLayout>

#include "qtkwidget.h"

namespace Ui {
  class DebugConsole;
}

namespace Qtk {
  class DebugConsole : public QDockWidget {
      Q_OBJECT;

    public:
      /**
       * Construct a new DebugConsole.
       * Assigns a default name to the console using `key + "Debugger"`
       *
       * @param owner Parent widget for this console or nullptr if no parent.
       *    If this parameter inherits from QMainWindow we will add this dock
       *    widget to the window.
       * @param key The objectName associated with the attached QtkWidget.
       */
      DebugConsole(QWidget * owner, const QString & key);

      /**
       * Construct a new DebugConsole.
       *
       * @param owner Parent widget for this console or nullptr if no parent.
       *    If this parameter inherits from QMainWindow we will add this dock
       *    widget to the window.
       * @param key The objectName associated with the attached QtkWidget.
       * @param name The objectName to associate with this DebugConsole.
       */
      DebugConsole(QWidget * owner, const QString & key, const QString & name);

      ~DebugConsole() = default;

    public slots:
      /*************************************************************************
       * Public Qt slots
       ************************************************************************/

      /**
       * Log a message to the DebugConsole text view.
       *
       * @param message The message to log.
       * @param context The DebugContext to use for the message.
       *    Default value is Status.
       */
      inline void sendLog(QString message, DebugContext context = Status) {
        mConsole->setTextColor(logColor(context));
        mConsole->append(logPrefix(message, context));
      }

      /**
       * Sets the window title for the DebugConsole. This will appear in the
       * widget title bar and within any context menu actions.
       *
       * @param name Base name for the DebugConsole window.
       */
      inline void setTitle(QString name) {
        setWindowTitle(name + " Debug Console");
      }

    private:
      /**
       * @param context Log context severity level.
       * @return QColor corresponding with the message context.
       */
      [[nodiscard]] QColor logColor(const DebugContext & context) const {
        switch(context) {
          case Status:
            return Qt::GlobalColor::darkGray;
          case Debug:
            return Qt::GlobalColor::white;
          case Warn:
            return Qt::GlobalColor::yellow;
          case Error:
            return Qt::GlobalColor::red;
          case Fatal:
            return Qt::GlobalColor::magenta;
          default:
            return Qt::GlobalColor::darkYellow;
        }
      }

      /**
       * Prefixes a log message to add context level.
       *
       * @param message The message to prefix.
       * @param context The log context severity level.
       * @return The log message prefixed with the DebugContext level.
       */
      [[nodiscard]] QString logPrefix(
          QString & message, const DebugContext & context) {
        QString prefix;
        switch(context) {
          case Status:
            prefix = "[Status]: ";
            break;
          case Debug:
            prefix = "[Debug]: ";
            break;
          case Warn:
            prefix = "[Warn]: ";
            break;
          case Error:
            prefix = "[Error]: ";
            break;
          case Fatal:
            prefix = "[Fatal]: ";
            break;
          default:
            prefix = "[No Context]: ";
            break;
        }
        message = prefix + message.replace("\n", "\t\n" + prefix);
        return message;
      }

      Ui::DebugConsole * ui_;
      QTextEdit * mConsole;
  };
}  // namespace Qtk

#endif  // QTK_DEBUGCONSOLE_H
