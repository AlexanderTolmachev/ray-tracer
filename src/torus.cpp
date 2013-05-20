#include "torus.h"
#include "rayintersection.h"

Torus::Torus(Vector center, Vector axis, float innerRadius, float outerRadius, MaterialPointer material) 
  : Shape(material),
    mCenter(center),
    mAxis(axis),
    mInnerRadius(innerRadius),
    mOuterRadius(outerRadius) {
  mAxis.normalize();
}

Torus::~Torus() {
}

RayIntersection Torus::intersectWithRay(const Ray &ray) const {
  // TODO: Implement
  return RayIntersection();
}

Vector Torus::getNormal(const Ray &ray, float distance) const {
  Vector point = ray.getPointAt(distance);
  Vector centerToPoint = point - mCenter;
  float	centerToPointDotAxis = centerToPoint.dotProduct(mAxis);
  Vector direction = centerToPoint - mAxis * centerToPointDotAxis;
  direction.normalize();
  Vector normal = point - mCenter + direction * mOuterRadius;
  normal.normalize();
  return normal;
}
