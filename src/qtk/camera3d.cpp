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
