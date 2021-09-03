/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2021 Shaun Reed, all rights reserved                ##
## About: Transform3D class to represent object position in 3D space          ##
##        From following tutorials at trentreed.net                           ##
##                                                                            ##
## Contact: shaunrd0@gmail.com	| URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_TRANSFORM3D_H
#define QTK_TRANSFORM3D_H

#include <QDebug>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>


class Transform3D
{
public:
  // Constructors
  inline Transform3D() : m_dirty(true),
                         mScale(1.0f, 1.0f, 1.0f),
                         mTranslation(0.0f, 0.0f, 0.0f) { }

  //
  // Transformations

  void translate(const QVector3D & dt);
  inline void translate(float dx, float dy, float dz)
  { translate(QVector3D(dx, dy, dz));}

  // Scale object with multiplication
  void scale(const QVector3D & ds);
  inline void scale(float dx, float dy, float dz)
  { scale(QVector3D(dx, dy, dz));}
  inline void scale(float factor)
  { scale(QVector3D(factor, factor, factor));}

  // Multiplying by a rotation
  void rotate(const QQuaternion & dr);
  inline void rotate(float angle, const QVector3D & axis)
  { rotate(QQuaternion::fromAxisAndAngle(axis, angle));}
  inline void rotate(float angle, float ax, float ay, float az)
  { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle));}

  // Scale object by addition
  void grow(const QVector3D & ds);
  inline void grow(float dx, float dy, float dz)
  { grow(QVector3D(dx, dy, dz));}
  inline void grow(float factor)
  { grow(QVector3D(factor, factor, factor));}

  //
  // Setters

  // Set object position
  void setTranslation(const QVector3D & t);
  inline void setTranslation(float x, float y, float z)
  { setTranslation(QVector3D(x, y, z));}

  // Set object scale
  void setScale(const QVector3D & s);
  inline void setScale(float x, float y, float z)
  { setScale(QVector3D(x, y, z));}
  inline void setScale(float k)
  { setScale(QVector3D(k, k, k));}

  // Set object rotation
  void setRotation(const QQuaternion & r);
  inline void setRotation(float angle, const QVector3D & axis)
  { setRotation(QQuaternion::fromAxisAndAngle(axis, angle));}
  inline void setRotation(float angle, float ax, float ay, float az)
  { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle));}

  //
  // Accessors

  inline const QVector3D & translation() const { return mTranslation;}
  inline const QVector3D & scale() const { return mScale; }
  inline const QQuaternion & rotation() const { return mRotation; }
  const QMatrix4x4 & toMatrix();

  QVector3D forward() const;
  QVector3D up() const;
  QVector3D right() const;

  static const QVector3D LocalForward, LocalUp, LocalRight;

private:
  QVector3D mTranslation;
  QQuaternion mRotation;
  QVector3D mScale;
  QMatrix4x4 mWorld;

  bool m_dirty;

#ifndef QT_NO_DATASTREAM
  friend QDataStream &operator<<(QDataStream & out, const Transform3D & transform);
  friend QDataStream &operator>>(QDataStream & in, Transform3D & transform);
#endif
};

Q_DECLARE_TYPEINFO(Transform3D, Q_MOVABLE_TYPE);

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const Transform3D & transform);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream & out, const Transform3D & transform);
QDataStream &operator>>(QDataStream & in, Transform3D & transform);
#endif

#endif // QTK_TRANSFORM3D_H
