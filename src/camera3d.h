/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2022 Shaun Reed, all rights reserved                ##
## About: Fly camera class from tutorials followed at trentreed.net           ##
##                                                                            ##
## Contact: shaunrd0@gmail.com	| URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_CAMERA3D_H
#define QTK_CAMERA3D_H

#include <QDebug>

#include <qtkapi.h>
#include <transform3D.h>

namespace Qtk {
  class QTKAPI Camera3D {
    public:
      /*************************************************************************
       * Constants
       ************************************************************************/

      static const QVector3D LocalForward;
      static const QVector3D LocalUp;
      static const QVector3D LocalRight;

      /*************************************************************************
       * Accessors
       ************************************************************************/

      inline Transform3D & getTransform() { return mTransform; }

      [[nodiscard]] inline const QVector3D & getTranslation() const {
        return mTransform.getTranslation();
      }

      [[nodiscard]] inline const QQuaternion & getRotation() const {
        return mTransform.getRotation();
      }

      const QMatrix4x4 & toMatrix();

      // Queries
      [[nodiscard]] inline QVector3D forward() const {
        return mTransform.getRotation().rotatedVector(LocalForward);
      }

      [[nodiscard]] inline QVector3D right() const {
        return mTransform.getRotation().rotatedVector(LocalRight);
      }

      [[nodiscard]] inline QVector3D up() const {
        return mTransform.getRotation().rotatedVector(LocalUp);
      }

    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/

      Transform3D mTransform;
      QMatrix4x4 mWorld;

      /*************************************************************************
       * Private Methods
       ************************************************************************/

#ifndef QT_NO_DATASTREAM
      friend QDataStream & operator<<(QDataStream & out, Camera3D & transform);
      friend QDataStream & operator>>(QDataStream & in, Camera3D & transform);
#endif
  };

// Qt Streams
#ifndef QT_NO_DATASTREAM
  QDataStream & operator<<(QDataStream & out, const Camera3D & transform);
  QDataStream & operator>>(QDataStream & in, Camera3D & transform);
#endif

#ifndef QT_NO_DEBUG_STREAM
  QDebug operator<<(QDebug dbg, const Camera3D & transform);
#endif
}  // namespace Qtk

Q_DECLARE_TYPEINFO(Qtk::Camera3D, Q_MOVABLE_TYPE);

#endif  // QTK_CAMERA3D_H
