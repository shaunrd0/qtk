
#ifndef QTK_QTKMESSAGELOGGER_H
#define QTK_QTKMESSAGELOGGER_H

#include "qtkapi.h"

#include <QDebug>
#include <QMessageLogger>
#include <Qt>

// #define qtkDebug    Qtk::QtkMessageLogger::get().debug
// #define qtkInfo     Qtk::QtkMessageLogger::get().info
// #define qtkWarning  Qtk::QtkMessageLogger::get().warning
// #define qtkCritical Qtk::QtkMessageLogger::get().critical
// #define qtkFatal    Qtk::QtkMessageLogger::get().fatal

#define qtkDebug                                                  \
  Qtk::QtkMessageLogger(                                          \
      QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC) \
      .logger()                                                   \
      .debug
#define qtkInfo                                                   \
  Qtk::QtkMessageLogger(                                          \
      QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC) \
      .logger()                                                   \
      .info
#define qtkWarning                                                \
  Qtk::QtkMessageLogger(                                          \
      QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC) \
      .logger()                                                   \
      .warning
#define qtkCritical                                               \
  Qtk::QtkMessageLogger(                                          \
      QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC) \
      .logger()                                                   \
      .critical
#define qtkFatal                                                  \
  Qtk::QtkMessageLogger(                                          \
      QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC) \
      .logger()                                                   \
      .fatal

namespace Qtk {
  class Transform3D;
  class Camera3D;

  class QTKAPI QtkDebug : public QDebug {
    public:
      QtkDebug operator<<(const Transform3D & transform);
  };

#ifndef QT_NO_DEBUG_STREAM
  QDebug operator<<(QDebug dbg, const Transform3D & transform);

  QDebug operator<<(QDebug dbg, const Camera3D & camera);
#endif

#ifndef QT_NO_DATASTREAM
  QDataStream & operator<<(QDataStream & out, const Transform3D & transform);
  QDataStream & operator>>(QDataStream & in, Transform3D & transform);

  QDataStream & operator<<(QDataStream & out, const Camera3D & camera);
  QDataStream & operator>>(QDataStream & in, Camera3D & camera);
#endif

  class QTKAPI QtkMessageLogger {
    public:
      QtkMessageLogger(const char * file, int line, const char * function) :
          mQMessageLogger(file, line, function) {
        qDebug();
      }

      //      static QtkMessageLogger * get() {
      //        if(mQtkMessageLogger == Q_NULLPTR) {
      //          mQtkMessageLogger = new QtkMessageLogger();
      //        }
      //        return mQtkMessageLogger;
      //      }

      //      QtkDebug debug() const {}

      [[nodiscard]] inline const QMessageLogger & logger() const {
        return mQMessageLogger;
      }

      inline QtkDebug debug() const { return {}; };


      static QtkMessageLogger * mQtkMessageLogger;
      QMessageLogger mQMessageLogger;
  };


}  // namespace Qtk

#endif  // QTK_QTKMESSAGELOGGER_H
