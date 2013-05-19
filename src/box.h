#pragma once

#include "shape.h"

class Box;

typedef QSharedPointer<Box> BoxPointer;

class Box : public Shape {
  public:
    Box(Vector min, Vector max, MaterialPointer material);
    virtual ~Box();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance) const;

  private:
    Vector mMin;
    Vector mMax;
};