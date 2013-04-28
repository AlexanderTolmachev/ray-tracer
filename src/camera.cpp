/*!
 *\file camera.cpp
 *\brief Contains Camera class definition
 */

#include "camera.h"

Camera::Camera(Vector position, Vector up, Vector lookAt, 
               float fov, float nearPlaneDistance, 
               int imagePlaneWidth, int imagePlaneHeight) 
  : mPosition(position),
    mUp(up),
    mLookAt(lookAt),
    mFov(fov),
    mNearPlaneDistance(nearPlaneDistance),
    mImagePlaneWidth(imagePlaneWidth),
    mImagePlaneHeight(imagePlaneHeight) {
  // TODO init other fields
}

Camera::~Camera() {
}

Ray Camera::emitRay(int x, int y) const {
  Ray ray;
  // TODO implement
  return ray;
}