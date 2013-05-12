#include "meshmodel.h"
#include "rayintersection.h"
#include "types.h"

bool BoundingBox::intersectsWithRay(const Ray &ray) const {
  Vector rayOrigin    = ray.getOriginPosition();
  Vector rayDirection = ray.getDirection();

  float d0 = -MAX_DISTANCE_TO_INTERSECTON;
  float d1 = MAX_DISTANCE_TO_INTERSECTON;

  if (fabs(rayDirection.x) > FLOAT_ZERO) {
    d0 = (min.x - rayOrigin.x) / rayDirection.x;
    d1 = (max.x - rayOrigin.x) / rayDirection.x;
    if(d1 < d0) {
      std::swap(d0, d1);
    }
  }

  if (fabs(rayDirection.y) > FLOAT_ZERO) {
    float t0 = (min.y - rayOrigin.y) / rayDirection.y;
    float t1 = (max.y - rayOrigin.y) / rayDirection.y;
    
    if(t1 < t0) {
      std::swap(t0, t1);
    }
    d0 = std::max(d0, t0);
    d1 = std::min(d1, t1);
  }

  if (fabs(rayDirection.z) > FLOAT_ZERO) {
    float t0 = (min.z - rayOrigin.z) / rayDirection.z;
    float t1 = (max.z - rayOrigin.z) / rayDirection.z;
    if(t1 < t0) {
      std::swap(t0, t1);
    }
    d0 = std::max(d0, t0);
    d1 = std::min(d1, t1);
  }

  if (d1 < d0 || d0 == -MAX_DISTANCE_TO_INTERSECTON) {
    return false;
  } else {
    return true;
  }
}

MeshModel::MeshModel(const std::vector<TrianglePointer> &triangles, const BoundingBox &boundingBox, MaterialPointer material)
  : Shape(material),
    mTriangles(triangles),
    mBoundingBox(boundingBox) {
}

RayIntersection MeshModel::intersectWithRay(const Ray &ray) const {
  if (!mBoundingBox.intersectsWithRay(ray)) {
    return RayIntersection();
  }

  RayIntersection closestIntersection;
  for each (auto triangle in mTriangles) {
    RayIntersection currentIntersection = triangle->intersectWithRay(ray);
    if (currentIntersection.rayIntersectsWithShape) {
      if (currentIntersection.distanceFromRayOrigin < closestIntersection.distanceFromRayOrigin) {
        closestIntersection.rayIntersectsWithShape = true;
        closestIntersection.distanceFromRayOrigin = currentIntersection.distanceFromRayOrigin;
        closestIntersection.shape	= MeshModelPointer(new MeshModel(*this));
        closestIntersection.normalAtInresectionPoint = currentIntersection.normalAtInresectionPoint;
      }
    }
  }

  return closestIntersection;
}

Vector MeshModel::getNormal(const Ray &ray, float distance) const {
  // This method is actually never called
  return Vector();
}
