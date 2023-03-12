/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Fly camera class from tutorials followed at trentreed.net           ##
##                                                                            ##
## Contact: shaunrd0@gmail.com	| URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "camera3d.h"

using namespace Qtk;

/*******************************************************************************
 * Static Public Constants
 ******************************************************************************/

const QVector3D Camera3D::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera3D::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Camera3D::LocalRight(1.0f, 0.0f, 0.0f);

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

const QMatrix4x4 & Camera3D::toMatrix() {
  mWorld.setToIdentity();
  // Qt6 renamed QMatrix4x4::conjugate() to conjugated()
  mWorld.rotate(mTransform.getRotation().conjugated());
  mWorld.translate(-mTransform.getTranslation());
  return mWorld;
}

/*******************************************************************************
 * Qt Streams
 ******************************************************************************/

QDataStream & operator<<(QDataStream & out, Camera3D & transform) {
  out << transform.getTransform();
  return out;
}

QDataStream & operator>>(QDataStream & in, Camera3D & transform) {
  in >> transform.getTransform();
  return in;
}

QDebug operator<<(QDebug dbg, const Camera3D & transform) {
  dbg << "Camera3D\n{\n";
  dbg << "Position: <" << transform.getTranslation().x() << ", "
      << transform.getTranslation().y() << ", "
      << transform.getTranslation().z() << ">\n";
  dbg << "Rotation: <" << transform.getRotation().x() << ", "
      << transform.getRotation().y() << ", " << transform.getRotation().z()
      << " | " << transform.getRotation().scalar() << ">\n}";
  return dbg;
}
