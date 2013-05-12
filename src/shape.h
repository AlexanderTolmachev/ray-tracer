/*!
 *\file shape.h
 *\brief Contains abstract Shape class definition
 */

#pragma once

#include "types.h"
#include "material.h"
#include "ray.h"

class Shape;
struct RayIntersection; 

typedef QSharedPointer<Shape> ShapePointer;

class Shape {
  public:
    Shape(MaterialPointer material) :mMaterial(material) {} 
    virtual ~Shape() {}

    virtual RayIntersection intersectWithRay(const Ray &ray) const = 0;
    virtual Vector getNormal(const Ray &ray, float distance, const RayIntersection &intersection) const = 0;

    MaterialPointer getMaterial() const { return mMaterial; }
    
  private:
    MaterialPointer mMaterial;
};