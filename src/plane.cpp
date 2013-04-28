/*!
 *\file plane.cpp
 *\brief Contains Plane class definition
 */

#include "plane.h"

Plane::Plane(MaterialPointer material, const Vector &normal, float distance) 
  : Shape(material),
    mNormal(normal),
    mDistance(distance) {}

Plane::~Plane() {
}

RayIntersection Plane::intersectWithRay(const Ray &ray) const {
  // TODO implement
  return RayIntersection();
}

Vector Plane::getNormal(const Ray &ray, float distance) const {
  return mNormal;
}