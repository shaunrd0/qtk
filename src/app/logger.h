/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Logger for Qtk                                                      ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_LOGGER_H
#define QTK_LOGGER_H

#include <QOpenGLDebugMessage>
#include <QString>
#include "qtk/qtkapi.h"
namespace Qtk {
  class QtkWidget;

  class Logger : public QObject {
      Q_OBJECT

    public:
      explicit Logger(Qtk::QtkWidget * parent = nullptr);

    public slots:
      std::pair<QString, DebugContext> log(
          const QOpenGLDebugMessage & msg);

      void log(
          const QString & message,
          Qtk::DebugContext context = Qtk::DebugContext::Status);

    signals:
      void sendLog(const QString & message, Qtk::DebugContext context);
  };
}  // namespace Qtk

#endif  // QTK_LOGGER_H
