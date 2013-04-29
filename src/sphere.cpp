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
  // Solve square equation x^2 + b * x + c = 0
  Vector cameraToRayOrigin = ray.getOriginPosition() - mCenter;
  float b = ray.getDirection().dotProduct(cameraToRayOrigin);
  float c = cameraToRayOrigin.dotProduct(cameraToRayOrigin) - mRadius * mRadius;
  float descriminant = b * b - c;

  if (descriminant < 0)
  {
    return RayIntersection();
  }

  descriminant = sqrt(descriminant);

  float	closestRoot = -1.f;

  // Get closest root
  float root = -b - descriminant;
//  float rayExit = -1.f;
  if (root >= 0.f)
  {
    closestRoot = root;
  }

  root = -b + descriminant;
  if (root >= 0.f)
  {
    if (closestRoot < 0.f)
    {
      closestRoot = root;
    }
    else if (root < closestRoot)
    {
//      rayExit = closestRoot;
      closestRoot = root;
    }
    //else
    //{
    //  rayExit = root;
    //}
  }

  if (closestRoot > 0.f)
  {
    SpherePointer pointer = SpherePointer(new Sphere(*this));
    return RayIntersection(true, pointer, closestRoot, getNormal(ray, closestRoot));
  }

  return RayIntersection();
}

Vector Sphere::getNormal(const Ray &ray, float distance) const {
  Vector normal = (ray.getPointAt(distance) - mCenter) / mRadius;
  normal.normalize();
  return normal;
}
