#pragma once

#include "shape.h"

class Torus;

typedef QSharedPointer<Torus> TorusPointer;

class Torus : public Shape {
  public:
    Torus(Vector center, Vector axis, float innerRadius, float outerRadius, MaterialPointer material);
    virtual ~Torus();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance) const;

  private:
    Vector mCenter;
    Vector mAxis;
    float mInnerRadius;
    float mOuterRadius;
};