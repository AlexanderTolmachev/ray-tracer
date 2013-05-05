/*!
 *\file plane.h
 *\brief Contains Cylinder class declaration
 */

#pragma once

#include "shape.h"
#include "types.h"

class Cylinder;

typedef QSharedPointer<Cylinder> CylinderPointer;

class Cylinder : public Shape {
  public:
    Cylinder(Vector topCenter, Vector bottomCenter, float radius, MaterialPointer material);
    virtual ~Cylinder();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance) const;

  private:
    Vector mBottomCenter;
    Vector mTopCenter;
    float mRadius;
};