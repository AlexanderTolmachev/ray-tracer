/*!
 *\file plane.h
 *\brief Contains Plane class declaration
 */

#pragma once

#include "shape.h"
#include "types.h"

class Plane : public Shape {
public:
  Plane(MaterialPointer material, const Vector &normal, float distance);
  virtual ~Plane();

  virtual RayIntersection intersectWithRay(const Ray &ray) const;
  virtual Vector getNormal(const Ray &ray, float distance) const;

private:
  Vector mNormal;
  float mDistance;
};