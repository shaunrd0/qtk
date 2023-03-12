/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Transform3D class to represent object position in 3D space          ##
##        From following tutorials at trentreed.net                           ##
##                                                                            ##
## Contact: shaunrd0@gmail.com	| URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_TRANSFORM3D_H
#define QTK_TRANSFORM3D_H

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#endif

#include "qtkapi.h"

namespace Qtk {
  /**
   * Transform3D class to represent and modify object position in 3D space.
   */
  class QTKAPI Transform3D {
    public:
      /*************************************************************************
       * Constructors, Destructors
       ************************************************************************/

      inline Transform3D() :
          m_dirty(true), mScale(1.0f, 1.0f, 1.0f),
          mTranslation(0.0f, 0.0f, 0.0f) {}

      /*************************************************************************
       * Public Methods
       ************************************************************************/

      /**
       * @param dt Translation from last to current position.
       */
      void translate(const QVector3D & dt);

      /**
       * @param dx X translation from last to current position.
       * @param dy Y translation from last to current position.
       * @param dz Z translation from last to current position.
       */
      inline void translate(float dx, float dy, float dz) {
        translate(QVector3D(dx, dy, dz));
      }

      /**
       * Scale the object size.
       *
       * @param ds Scalar vector to apply to the transform.
       */
      void scale(const QVector3D & ds);

      /**
       * Scale the object size.
       *
       * @param dx Amount to scale on the X axis.
       * @param dy Amount to scale on the Y axis.
       * @param dz Amount to scale on the Z axis.
       */
      inline void scale(float dx, float dy, float dz) {
        scale(QVector3D(dx, dy, dz));
      }

      /**
       * Scale the object size.
       *
       * @param factor Scalar to apply to all axis of the object.
       */
      inline void scale(float factor) {
        scale(QVector3D(factor, factor, factor));
      }

      /**
       * @param ds 3D vector to add to scale axis.
       */
      void grow(const QVector3D & ds);

      /**
       * @param dx Amount to grow X axis.
       * @param dy Amount to grow Y axis.
       * @param dz Amount to grow Z axis.
       */
      inline void grow(float dx, float dy, float dz) {
        grow(QVector3D(dx, dy, dz));
      }

      /**
       * @param factor Amount to grow all axis equally.
       */
      inline void grow(float factor) {
        grow(QVector3D(factor, factor, factor));
      }

      /**
       * @param dr Rotation to apply to the transform.
       */
      void rotate(const QQuaternion & dr);

      /**
       * @param angle Angle to rotate.
       * @param axis Axis to rotate apply the rotation on.
       */
      inline void rotate(float angle, const QVector3D & axis) {
        rotate(QQuaternion::fromAxisAndAngle(axis, angle));
      }

      /**
       * Apply rotation upon an axis represented by the 3D vector (x, y, z)
       *
       * @param angle Angle to rotate.
       * @param ax X axis to apply the rotation on.
       * @param ay Y axis to apply the rotation on.
       * @param az Z axis to apply the rotation on.
       */
      inline void rotate(float angle, float ax, float ay, float az) {
        rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle));
      }

      /*************************************************************************
       * Setters
       ************************************************************************/

      /**
       * @param t Position to move the transform to.
       */
      void setTranslation(const QVector3D & t);

      /**
       * @param x X position to set transform.
       * @param y Y position to set transform.
       * @param z Z position to set transform.
       */
      inline void setTranslation(float x, float y, float z) {
        setTranslation(QVector3D(x, y, z));
      }

      /**
       * @param s Scale to set for this transform.
       */
      void setScale(const QVector3D & s);

      /**
       * @param x X axis scale to set for this transform.
       * @param y Y axis scale to set for this transform.
       * @param z Z axis scale to set for this transform.
       */
      inline void setScale(float x, float y, float z) {
        setScale(QVector3D(x, y, z));
      }

      /**
       * @param k Scale to set for all axis on this transform.
       */
      inline void setScale(float k) { setScale(QVector3D(k, k, k)); }

      /**
       * @param r Rotation to set for this transform.
       */
      void setRotation(const QQuaternion & r);

      /**
       * @param angle Angle to set for rotation.
       * @param axis Axis to set rotation for.
       */
      inline void setRotation(float angle, const QVector3D & axis) {
        setRotation(QQuaternion::fromAxisAndAngle(axis, angle));
      }

      /**
       * Sets a rotation upon an axis represented by the 3D vector (x, y, z)
       *
       * @param angle Angle to set rotation.
       * @param ax X axis to set angle for.
       * @param ay Y axis to set angle for.
       * @param az Z axis to set angle for.
       */
      inline void setRotation(float angle, float ax, float ay, float az) {
        setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle));
      }

      /*************************************************************************
       * Getters
       ************************************************************************/

      /**
       * @return Translation for this transform.
       */
      [[nodiscard]] inline const QVector3D & getTranslation() const {
        return mTranslation;
      }

      /**
       * @return Scale for this transform.
       */
      [[nodiscard]] inline const QVector3D & getScale() const { return mScale; }

      /**
       * @return Rotation for this transform.
       */
      [[nodiscard]] inline const QQuaternion & getRotation() const {
        return mRotation;
      }

      /**
       * @return Model to world matrix for this transform.
       *    transformation * rotation * scale = ModelToWorld
       */
      const QMatrix4x4 & toMatrix();

      /**
       * @return Forward vector for this transform.
       */
      [[nodiscard]] QVector3D getForward() const;

      /**
       * @return Up vector for this transform.
       */
      [[nodiscard]] QVector3D getUp() const;

      /**
       * @return Right vector for this transform.
       */
      [[nodiscard]] QVector3D getRight() const;

      /*************************************************************************
       * Public Members
       ************************************************************************/

      static const QVector3D LocalForward, LocalUp, LocalRight;

    private:
      /*************************************************************************
       * Private Members
       ************************************************************************/

      QVector3D mTranslation;
      QQuaternion mRotation;
      QVector3D mScale;
      QMatrix4x4 mWorld;

      bool m_dirty;

#ifndef QT_NO_DATASTREAM
      friend QDataStream & operator<<(
          QDataStream & out, const Transform3D & transform);
      friend QDataStream & operator>>(
          QDataStream & in, Transform3D & transform);
#endif
  };

#ifndef QT_NO_DEBUG_STREAM
  QDebug operator<<(QDebug dbg, const Transform3D & transform);
#endif

#ifndef QT_NO_DATASTREAM
  QDataStream & operator<<(QDataStream & out, const Transform3D & transform);
  QDataStream & operator>>(QDataStream & in, Transform3D & transform);
#endif
}  // namespace Qtk

Q_DECLARE_TYPEINFO(Qtk::Transform3D, Q_MOVABLE_TYPE);

#endif  // QTK_TRANSFORM3D_H
