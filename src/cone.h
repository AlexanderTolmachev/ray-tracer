/*!
 *\file cone.h
 *\brief Contains Cone class declaration
 */

#pragma  once

#include "shape.h"
#include "types.h"

class Cone;

typedef QSharedPointer<Cone> ConePointer;

class Cone : public Shape {
  public:
    Cone(Vector top, Vector bottomCenter, float radius, MaterialPointer material);
    ~Cone();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance) const;

  private:
    Vector mTop;
    Vector mBottomCenter;
    float mRadius;
};