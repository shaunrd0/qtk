
#include "qtkmessagelogger.h"
#include "camera3d.h"
#include "transform3D.h"


Qtk::QtkDebug Qtk::QtkDebug::operator<<(const Qtk::Transform3D & transform) {}

#ifndef QT_NO_DEBUG_STREAM
QDebug Qtk::operator<<(QDebug dbg, const Qtk::Transform3D & transform) {
  dbg << "Transform3D\n{\n";
  dbg << "Position: <" << transform.getTranslation().x() << ", "
      << transform.getTranslation().y() << ", "
      << transform.getTranslation().z() << ">\n";
  dbg << "Scale: <" << transform.getScale().x() << ", "
      << transform.getScale().y() << ", " << transform.getScale().z() << ">\n";
  dbg << "Rotation: <" << transform.getRotation().x() << ", "
      << transform.getRotation().y() << ", " << transform.getRotation().z()
      << " | " << transform.getRotation().scalar() << ">\n}";
  return dbg;
}

QDebug Qtk::operator<<(QDebug dbg, const Qtk::Camera3D & transform) {
  dbg << "Camera3D\n{\n";
  dbg << "Position: <" << transform.getTranslation().x() << ", "
      << transform.getTranslation().y() << ", "
      << transform.getTranslation().z() << ">\n";
  dbg << "Rotation: <" << transform.getRotation().x() << ", "
      << transform.getRotation().y() << ", " << transform.getRotation().z()
      << " | " << transform.getRotation().scalar() << ">\n}";
  return dbg;
}
#endif

#ifndef QT_NO_DATASTREAM
QDataStream & Qtk::operator<<(
    QDataStream & out, const Qtk::Transform3D & transform) {
  out << transform.mTranslation;
  out << transform.mScale;
  out << transform.mRotation;
  return out;
}

QDataStream & Qtk::operator>>(QDataStream & in, Qtk::Transform3D & transform) {
  in >> transform.mTranslation;
  in >> transform.mScale;
  in >> transform.mRotation;
  transform.m_dirty = true;
  return in;
}

QDataStream & Qtk::operator<<(QDataStream & out, const Qtk::Camera3D & camera) {
  out << camera.getTransform();
  return out;
}

QDataStream & Qtk::operator>>(QDataStream & in, Qtk::Camera3D & camera) {
  in >> camera.mTransform;
  return in;
}
#endif