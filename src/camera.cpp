/*!
 *\file camera.cpp
 *\brief Contains Camera class definition
 */

#include "camera.h"

Camera::Camera(Vector position, Vector up, Vector lookAt, 
               float fov, float nearPlaneDistance) 
  : mPosition(position),
    mUp(up),
    mLookAt(lookAt),
    mFov(fov),
    mNearPlaneDistance(nearPlaneDistance) {
  // TODO init other fields
}

Camera::~Camera() {
}

Ray Camera::emitRay(int x, int y) const {
  Ray ray;
  // TODO implement
  return ray;
}

void Camera::setImagePlaneResolution(int width, int height) {
  mImagePlaneWidth = width;
  mImagePlaneHeight = height;
  mAspectRatio = static_cast<float>(mImagePlaneWidth) / mImagePlaneHeight;
}