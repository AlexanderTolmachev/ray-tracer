/*!
 *\file shape.h
 *\brief Contains abstract Shape class definition
 */

#pragma once

#include "types.h"
#include "material.h"
#include "rayintersection.h"
#include "ray.h"

class Shape;

typedef QSharedPointer<Shape> ShapePointer;

class Shape {
  public:
    Shape(MaterialPointer material) :mMaterial(material) {} 
    virtual ~Shape() {}

    virtual RayIntersection intersectWithRay(const Ray &ray) const = 0;
    virtual Vector getNormal(const Ray &ray, float distance) const = 0;

    Color getAmbientColor() const  { return mMaterial->ambientColor; }
    Color getDiffuseColor() const  { return mMaterial->diffuseColor; }
    Color getSpecularColor() const { return mMaterial->specularColor; }

    float getReflectionCoefficient() const { return mMaterial->reflectionCoefficient; }
    float getRefractionCoefficient() const { return mMaterial->refractionCoefficient; }

  private:
    MaterialPointer mMaterial;
};