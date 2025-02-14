/*##############################################################################
## Author: Shaun Reed                                                         ##
## Legal: All Content (c) 2023 Shaun Reed, all rights reserved                ##
## About: Transform3D class to represent object position in 3D space          ##
##        From following tutorials at trentreed.net                           ##
##                                                                            ##
## Contact: shaunrd0@gmail.com	| URL: www.shaunreed.com | GitHub: shaunrd0   ##
##############################################################################*/

#include "transform3D.h"

using namespace Qtk;

const QVector3D Transform3D::LocalForward(0.0f, 0.0f, 1.0f);
const QVector3D Transform3D::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Transform3D::LocalRight(1.0f, 0.0f, 0.0f);

/*******************************************************************************
 * Public Methods
 ******************************************************************************/

void Transform3D::translate(const QVector3D & dt) {
  m_dirty = true;
  mTranslation += dt;
}

void Transform3D::scale(const QVector3D & ds) {
  m_dirty = true;
  mScale *= ds;
}


void Transform3D::grow(const QVector3D & ds) {
  m_dirty = true;
  mScale += ds;
}

void Transform3D::rotate(const QQuaternion & dr) {
  m_dirty = true;
  mRotation = dr * mRotation;
}

void Transform3D::setTranslation(const QVector3D & t) {
  m_dirty = true;
  qtkDebug() << "Setting translation to " << t;
  mTranslation = t;
}

void Transform3D::setScale(const QVector3D & s) {
  m_dirty = true;
  mScale = s;
}

void Transform3D::setRotation(const QQuaternion & r) {
  m_dirty = true;
  mRotation = r;
}

const QMatrix4x4 & Transform3D::toMatrix() {
  if(m_dirty) {
    m_dirty = false;
    mWorld.setToIdentity();
    mWorld.translate(mTranslation);
    mWorld.rotate(mRotation);
    mWorld.scale(mScale);
  }
  return mWorld;
}

QVector3D Transform3D::getForward() const {
  return mRotation.rotatedVector(LocalForward);
}

QVector3D Transform3D::getUp() const {
  return mRotation.rotatedVector(LocalUp);
}

QVector3D Transform3D::getRight() const {
  return mRotation.rotatedVector(LocalRight);
}
