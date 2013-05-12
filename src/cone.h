/*!
 *\file cone.h
 *\brief Contains Cone class declaration
 */

#pragma  once

#include "shape.h"
#include "types.h"
#include "rayintersection.h"

class Cone;

typedef QSharedPointer<Cone> ConePointer;

class Cone : public Shape {
  public:
    Cone(Vector top, Vector bottomCenter, float radius, MaterialPointer material);
    virtual ~Cone();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance, const RayIntersection &intersection = RayIntersection()) const;

  private:
    Vector mTop;
    Vector mBottomCenter;
    float mRadius;
};