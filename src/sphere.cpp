/*!
 *\file shere.cpp
 *\brief Contains Sphere class definition
 */

#include "sphere.h"
#include "rayintersection.h"

Sphere::Sphere(Vector center, float radius, MaterialPointer material) 
  : Shape(material), 
    mCenter(center), 
    mRadius(radius) {
}

Sphere::~Sphere() {
}

RayIntersection Sphere::intersectWithRay(const Ray &ray) const {
  Vector centerToOriginVector = ray.getOriginPosition() - mCenter;

  // Solving equation x^2 + b * x + c = 0
  float a = 1;
  float b = ray.getDirection().dotProduct(centerToOriginVector);
  float c = centerToOriginVector.dotProduct(centerToOriginVector) - mRadius * mRadius;  
  Roots roots;
  int num_roots = solveQuadraticEquation(a, b, c, roots);

  if (num_roots == 0) {
    return RayIntersection();
  }

  float	distance = getPositiveClosestToZeroRoot(roots);

  if (distance > 0)
  {
    SpherePointer pointer = SpherePointer(new Sphere(*this));
    return RayIntersection(true, pointer, distance, getNormal(ray, distance));
  }

  return RayIntersection();
}

Vector Sphere::getNormal(const Ray &ray, float distance) const {
  Vector normal = (ray.getPointAt(distance) - mCenter) / mRadius;
  normal.normalize();
  return normal;
}

/*
* private:
*/
int Sphere::solveQuadraticEquation(float a, float b, float c, Roots &roots) const {
  float discreminant = 0.f;
  float divider = 0.f;

  discreminant = b * b - a * c;

  if (discreminant < 0) {
    return 0;
  }

  discreminant = sqrt(discreminant);
  divider = 1.f / a;

  roots.x = (b - discreminant) * divider;
  roots.y = (b + discreminant) * divider;

  if (discreminant == 0) {
    return 1;
  }

  return 2;
}

float Sphere::getPositiveClosestToZeroRoot(const Roots &roots) const {
  float closest = -1.f;

  if (roots.x > 0) {
    closest = roots.x;
  }
  if (roots.y > 0 && roots.y < closest) {
     closest = roots.y;
  }
  
  return closest;
}