/*!
 *\file shere.h
 *\brief Contains Sphere class declaration
 */
#pragma once;

#include "shape.h"
#include "rayintersection.h"

class Sphere;

typedef QSharedPointer<Sphere> SpherePointer;

class Sphere : public Shape {
  public:
    Sphere(Vector center, float radius, MaterialPointer material);
    virtual ~Sphere();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance, const RayIntersection &intersection = RayIntersection()) const;

  private:
    Vector mCenter;
    float mRadius;
};