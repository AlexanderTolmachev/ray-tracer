/*!
 *\file shere.h
 *\brief Contains Sphere class declaration
 */
#pragma once;

#include "shape.h"

class Sphere;

typedef QSharedPointer<Sphere> SpherePointer;

class Sphere : public Shape {
  public:
    Sphere(Vector center, float radius, MaterialPointer material);
    virtual ~Sphere();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance) const;

  private:
    Vector mCenter;
    float mRadius;
};