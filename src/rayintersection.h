/*!
 *\file rayintersection.h
 *\brief Contains RayIntersection struct declaration
 */

#pragma once

#include <vector>

#include "types.h"
#include "shape.h"

struct RayIntersection {
  RayIntersection() 
    : rayIntersectsWithShape(false), 
      shape(NULL), 
      distanceFromRayOrigin(MAX_DISTANCE_TO_INTERSECTON) {}
  RayIntersection(bool intersectsWithShape, ShapePointer intersectsWith, float distance, Vector normal, std::vector<float> distances)
    : rayIntersectsWithShape(intersectsWithShape), 
      shape(intersectsWith), 
      distanceFromRayOrigin(distance), 
      normalAtInresectionPoint(normal),
      intersectionDistances(distances) {}
  // Does intersection exist
  bool rayIntersectsWithShape;
  // Shape the ray intersects with
  ShapePointer shape;
  // Distance from ray origin to intersection point
  float distanceFromRayOrigin;
  // Normal at intersection point
  Vector normalAtInresectionPoint;
  // Distances from ray origin of all ray intersections with shape (for CSG)
  std::vector<float> intersectionDistances;
};