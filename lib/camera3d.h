/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2021 Shaun Reed, all rights reserved                ##
## About: Fly camera class from tutorials followed at trentreed.net           ##
##                                                                            ##
## Contact: shaunrd0@gmail.com	| URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#ifndef QTK_CAMERA3D_H
#define QTK_CAMERA3D_H

#include <QDebug>

#include <transform3D.h>


class Camera3D {
public:
  // Constants
  static const QVector3D LocalForward;
  static const QVector3D LocalUp;
  static const QVector3D LocalRight;

  // Accessors
  inline Transform3D & transform() { return mTransform;}
  inline const QVector3D & translation() const
  { return mTransform.translation();}
  inline const QQuaternion & rotation() const
  { return mTransform.rotation();}
  const QMatrix4x4 & toMatrix();

  // Queries
  inline QVector3D forward() const
  { return mTransform.rotation().rotatedVector(LocalForward);}
  inline QVector3D right() const
  { return mTransform.rotation().rotatedVector(LocalRight);}
  inline QVector3D up() const
  { return mTransform.rotation().rotatedVector(LocalUp);}

private:
  Transform3D mTransform;
  QMatrix4x4 mWorld;

#ifndef QT_NO_DATASTREAM
  friend QDataStream & operator<<(QDataStream & out, Camera3D & transform);
  friend QDataStream & operator>>(QDataStream & in, Camera3D & transform);
#endif
};

Q_DECLARE_TYPEINFO(Camera3D, Q_MOVABLE_TYPE);

// Qt Streams
#ifndef QT_NO_DATASTREAM
QDataStream & operator<<(QDataStream & out, const Camera3D & transform);
QDataStream & operator>>(QDataStream & in, Camera3D & transform);
#endif

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const Camera3D & transform);
#endif

#endif // QTK_CAMERA3D_H
