/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Fly camera class from tutorials followed at trentreed.net           ##
##                                                                            ##
## Contact: shaunrd0@gmail.com	| URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_CAMERA3D_H
#define QTK_CAMERA3D_H

#include <QDebug>

#include "qtkapi.h"
#include "transform3D.h"

namespace Qtk {
  class QTKAPI Camera3D {
    public:
      /*************************************************************************
       * Static Public Constants
       ************************************************************************/

      static const QVector3D LocalForward;
      static const QVector3D LocalUp;
      static const QVector3D LocalRight;

      /*************************************************************************
       * Accessors
       ************************************************************************/

      /**
       * @return Transform3D associated with this camera.
       */
      inline Transform3D & getTransform() { return mTransform; }

      /**
       * @return Current translation of the camera as a QVector3D.
       */
      [[nodiscard]] inline const QVector3D & getTranslation() const {
        return mTransform.getTranslation();
      }

      /**
       * @return Current rotation of this camera as a QQuaternion.
       */
      [[nodiscard]] inline const QQuaternion & getRotation() const {
        return mTransform.getRotation();
      }

      /**
       * @return QVector3D for the forward vector of the camera.
       */
      [[nodiscard]] inline QVector3D getForward() const {
        return mTransform.getRotation().rotatedVector(LocalForward);
      }

      /**
       * @return QVector3D for the right vector of the camera.
       */
      [[nodiscard]] inline QVector3D getRight() const {
        return mTransform.getRotation().rotatedVector(LocalRight);
      }

      /**
       * @return QVector3D for the up vector of the camera.
       */
      [[nodiscard]] inline QVector3D getUp() const {
        return mTransform.getRotation().rotatedVector(LocalUp);
      }

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      /**
       * @return World To View matrix for this camera.
       */
      const QMatrix4x4 & toMatrix();

    private:
      /*************************************************************************
       * Private Methods
       ************************************************************************/

#ifndef QT_NO_DATASTREAM
      friend QDataStream & operator<<(QDataStream & out, Camera3D & transform);
      friend QDataStream & operator>>(QDataStream & in, Camera3D & transform);
#endif

      /*************************************************************************
       * Private Members
       ************************************************************************/

      Transform3D mTransform;
      QMatrix4x4 mWorld;
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
