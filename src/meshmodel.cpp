#include "meshmodel.h"
#include "rayintersection.h"
#include "types.h"

ModelTriangle::ModelTriangle(Vector vertex0, Vector vertex1, Vector vertex2,
                             Vector normal0, Vector normal1, Vector normal2, 
                             MaterialPointer material)
  : Triangle(vertex0, vertex1, vertex2, material),
    mNormal0(normal0),
    mNormal1(normal1),
    mNormal2(normal2) {
}

ModelTriangle::~ModelTriangle() {
}

Vector ModelTriangle::getNormal(const Ray &ray, float distance, float u, float v) const {
  Vector normal = mNormal1 * u + mNormal2 * v + mNormal0 * (1 - u - v);
  normal.normalize();
  return normal;
}

RayIntersection ModelTriangle::intersectWithRay(const Ray &ray) const {
  Vector rayOrigin	= ray.getOriginPosition();
  Vector rayDirection = ray.getDirection();

  Vector e1 = mVertex1 - mVertex0;
  Vector e2 = mVertex2 - mVertex0;

  Vector pvector = rayDirection.crossProduct(e2);
  float	determinant = e1.dotProduct(pvector);

  if (fabs(determinant) < FLOAT_ZERO) {
    return RayIntersection();
  }

  const float invertedDeterminant = 1.0 / determinant;

  Vector tvec	= rayOrigin - mVertex0;
  float	lambda = tvec.dotProduct(pvector);

  lambda *= invertedDeterminant;

  if (lambda < 0.0 || lambda > 1.0) {
    return RayIntersection();
  }

  Vector qvec = tvec.crossProduct(e1);
  float	mue	= rayDirection.dotProduct(qvec);

  mue *= invertedDeterminant;

  if (mue < 0.f || mue + lambda > 1.f) {
    return RayIntersection();
  }

  float f = e2.dotProduct(qvec);
  f = f * invertedDeterminant - FLOAT_ZERO;

  if (f < FLOAT_ZERO) {
    return RayIntersection();
  }

  std::vector<float> intersectionDistances;
  intersectionDistances.push_back(f);
  TrianglePointer pointer = TrianglePointer(new Triangle(*this));
  return RayIntersection(true, pointer, f, getNormal(ray, f, lambda, mue), intersectionDistances);
}


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

MeshModel::MeshModel(const std::vector<ModelTrianglePointer> &triangles, const BoundingBox &boundingBox, MaterialPointer material)
  : Shape(material),
    mTriangles(triangles),
    mBoundingBox(boundingBox) {
}

MeshModel::~MeshModel() {
}

RayIntersection MeshModel::intersectWithRay(const Ray &ray) const {
  if (!mBoundingBox.intersectsWithRay(ray)) {
    return RayIntersection();
  }

  RayIntersection closestIntersection;
  for each (auto triangle in mTriangles) {
    RayIntersection currentIntersection = triangle->intersectWithRay(ray);
    if (currentIntersection.rayIntersectsWithShape) {
      closestIntersection.intersectionDistances.push_back(currentIntersection.distanceFromRayOrigin);
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
