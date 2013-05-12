/*!
 *\file rayintersection.h
 *\brief Contains RayIntersection struct declaration
 */

#pragma once

#include "types.h"
#include "shape.h"

struct RayIntersection {
  RayIntersection() 
    : rayIntersectsWithShape(false), 
      shape(NULL), 
      distanceFromRayOrigin(MAX_DISTANCE_TO_INTERSECTON), 
      u(0.0), 
      v(0.0) {}
  RayIntersection(bool intersectsWithShape, ShapePointer intersectsWith, float distance, Vector normal, float uCoord = 0.0, float vCoord = 0.0)
    : rayIntersectsWithShape(intersectsWithShape), 
      shape(intersectsWith), 
      distanceFromRayOrigin(distance), 
      normalAtInresectionPoint(normal),
      u(uCoord),
      v(vCoord) {}
  // Does intersection exist
  bool rayIntersectsWithShape;
  // Shape the ray intersects with
  ShapePointer shape;
  // Distance from ray origin to intersection point
  float distanceFromRayOrigin;
  // Normal at intersection point
  Vector normalAtInresectionPoint;
  // Barycentric coordinates for intersections with triangles
  float	u;
  float	v;
};