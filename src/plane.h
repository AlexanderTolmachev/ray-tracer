/*!
 *\file plane.h
 *\brief Contains Plane class declaration
 */

#pragma once

#include "shape.h"
#include "types.h"
#include "rayintersection.h"

class Plane;

typedef QSharedPointer<Plane> PlanePointer;

class Plane : public Shape {
  public:
    Plane(const Vector &normal, float distance, MaterialPointer material);
    virtual ~Plane();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance, const RayIntersection &intersection = RayIntersection()) const;

  private:
    Vector mNormal;
    float mDistance;
};