/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Logger for Qtk                                                      ##
##                                                                            ##
## Contact: shaunrd0@gmail.com  | URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "logger.h"
#include "qtkwidget.h"

using namespace Qtk;

Logger::Logger(Qtk::QtkWidget * parent) : QObject(parent) {
  //  connect(
  //      this, SIGNAL(sendLog(QString, Qtk::DebugContext)), this,
  //      SLOT(log(QString, Qtk::DebugContext)));
  if(parent != Q_NULLPTR) {
    connect(
        parent, SIGNAL(sendLog(QString, Qtk::DebugContext)), this,
        SLOT(log(QString, Qtk::DebugContext)));
  }
}

std::pair<QString, DebugContext> Logger::log(const QOpenGLDebugMessage & msg) {
  QString error_msg;
  DebugContext context;

  // Format based on severity
  switch(msg.severity()) {
    case QOpenGLDebugMessage::NotificationSeverity:
      error_msg += "--";
      context = Status;
      break;
    case QOpenGLDebugMessage::HighSeverity:
      error_msg += "!!";
      context = Fatal;
      break;
    case QOpenGLDebugMessage::MediumSeverity:
      error_msg += "!~";
      context = Error;
      break;
    case QOpenGLDebugMessage::LowSeverity:
      error_msg += "~~";
      context = Warn;
      break;
    case QOpenGLDebugMessage::InvalidSeverity:
      error_msg += "??";
      context = Invalid;
      break;
    case QOpenGLDebugMessage::AnySeverity:
      error_msg += "**";
      context = Any;
      break;
  }

  error_msg += " (";

  // Format based on source
#define CASE(c)                \
  case QOpenGLDebugMessage::c: \
    error_msg += #c;           \
    break
  switch(msg.source()) {
    CASE(APISource);
    CASE(WindowSystemSource);
    CASE(ShaderCompilerSource);
    CASE(ThirdPartySource);
    CASE(ApplicationSource);
    CASE(OtherSource);
    CASE(InvalidSource);
    CASE(AnySource);
  }
#undef CASE

  error_msg += " : ";

// Format based on type
#define CASE(c)                \
  case QOpenGLDebugMessage::c: \
    error_msg += #c;           \
    break
  switch(msg.type()) {
    CASE(InvalidType);
    CASE(ErrorType);
    CASE(DeprecatedBehaviorType);
    CASE(UndefinedBehaviorType);
    CASE(PortabilityType);
    CASE(PerformanceType);
    CASE(OtherType);
    CASE(MarkerType);
    CASE(GroupPushType);
    CASE(GroupPopType);
    CASE(AnyType);
  }
#undef CASE

  error_msg += ")\n" + msg.message();
  log(error_msg, context);
  return {error_msg, context};
}

void Logger::log(const QString & message, DebugContext context) {
  const QString log_msg = "[LOGGER]: " + message + "; LOGGER";
  qDebug() << qPrintable(log_msg);
  emit sendLog(log_msg, context);
}
