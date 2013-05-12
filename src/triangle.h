/*!
 *\file triangle.h
 *\brief Contains Triangle class declaration
 */

#pragma once

#include "shape.h"
#include "types.h"
#include "rayintersection.h"

class Triangle;

typedef QSharedPointer<Triangle> TrianglePointer;

class Triangle : public Shape {
  public:
    Triangle(Vector vertex0, Vector vertex1, Vector vertex2, MaterialPointer material);
    virtual ~Triangle();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance, const RayIntersection &intersection = RayIntersection()) const;

  private:
    Vector mVertex0;
    Vector mVertex1;
    Vector mVertex2;
    Vector mNormal;
};