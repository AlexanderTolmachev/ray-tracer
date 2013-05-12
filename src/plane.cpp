/*!
 *\file plane.cpp
 *\brief Contains Plane class definition
 */

#include "plane.h"
#include "rayintersection.h"

Plane::Plane(const Vector &normal, float distance, MaterialPointer material) 
  : Shape(material),
    mNormal(normal),
    mDistance(distance) {}

Plane::~Plane() {
}

RayIntersection Plane::intersectWithRay(const Ray &ray) const {
  float cosineRayNormal = mNormal.dotProduct(ray.getDirection());
  if (fabs(cosineRayNormal) < FLOAT_ZERO) 
  {
    return RayIntersection();
  }
  
  float distance = -(ray.getOriginPosition().dotProduct(mNormal) + mDistance) / cosineRayNormal;
  if (distance > 0.0)
  {
    PlanePointer pointer = PlanePointer(new Plane(*this));
    return RayIntersection(true, pointer, distance, getNormal(ray, distance));
  }
  return RayIntersection();
}

Vector Plane::getNormal(const Ray &ray, float distance, const RayIntersection &intersection) const {
  return mNormal;
}