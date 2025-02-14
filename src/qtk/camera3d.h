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
      [[nodiscard]] inline const Transform3D & getTransform() const {
        return mTransform;
      }

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

      /**
       * Set the translation for this camera.
       * TODO: Replace these methods by inheriting from a base class.
       *
       * @param translation QVector3D for the new translation.
       */
      inline void setTranslation(const QVector3D & translation) {
        mTransform.setTranslation(translation);
      }

      /**
       * Set the rotation for this camera.
       *
       * @param rotation QQuaternion for the new rotation.
       */
      inline void setRotation(const QQuaternion & rotation) {
        mTransform.setRotation(rotation);
      }

      /**
       * Sets a rotation upon an axis represented by the 3D vector (x, y, z)
       *
       * @param ax X axis to set angle for.
       * @param ay Y axis to set angle for.
       * @param az Z axis to set angle for.
       * @param angle Angle to set rotation.
       */
      inline void setRotation(float ax, float ay, float az, float angle) {
        mTransform.setRotation(ax, ay, az, angle);
      }

      /**
       * Translate the camera by the given position.
       *
       * @param position QVector3D for the position to translate by.
       */
      inline void translate(const QVector3D & position) {
        mTransform.translate(position);
      }

      /**
       * Rotate the camera by the given rotation.
       *
       * @param rotation QQaternion for the rotation to apply.
       */
      inline void rotate(const QQuaternion & rotation) {
        mTransform.rotate(rotation);
      }

      /**
       * Sets a rotation upon an axis represented by the 3D vector (x, y, z)
       *
       * @param ax X axis to set angle for.
       * @param ay Y axis to set angle for.
       * @param az Z axis to set angle for.
       * @param angle Angle to set rotation.
       */
      inline void rotate(float ax, float ay, float az, float angle) {
        mTransform.rotate(ax, ay, az, angle);
      }

    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/

      Transform3D mTransform;
      QMatrix4x4 mWorld;

      /*************************************************************************
       * Qt Streams
       ************************************************************************/

#ifndef QT_NO_DATASTREAM
      friend QDataStream & operator<<(
          QDataStream & out, const Camera3D & camera);
      friend QDataStream & operator>>(QDataStream & in, Camera3D & camera);
#endif
  };
}  // namespace Qtk

Q_DECLARE_TYPEINFO(Qtk::Camera3D, Q_MOVABLE_TYPE);

#endif  // QTK_CAMERA3D_H
