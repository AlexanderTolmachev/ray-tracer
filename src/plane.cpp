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
  // TODO implement
  return RayIntersection();
}

Vector Plane::getNormal(const Ray &ray, float distance) const {
  return mNormal;
}