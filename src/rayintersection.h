/*!
 *\file rayintersection.h
 *\brief Contains RayIntersection struct declaration
 */

#pragma once

#include "types.h"
#include "shape.h"

#define MAX_DISTANCE_TO_INTERSECTON 1000000000.0

struct RayIntersection {
  RayIntersection() : rayIntersectsWithShape(false), shape(NULL), distanceFromRayOrigin(MAX_DISTANCE_TO_INTERSECTON) {}
  RayIntersection(bool intersectsWithShape, ShapePointer intersectsWith, float distance, Vector normal)
    : rayIntersectsWithShape(intersectsWithShape), shape(intersectsWith), distanceFromRayOrigin(distance), 
      normalAtInresectionPoint(normal) {}

  bool rayIntersectsWithShape;
  ShapePointer shape;
  float distanceFromRayOrigin;
  Vector normalAtInresectionPoint;
};