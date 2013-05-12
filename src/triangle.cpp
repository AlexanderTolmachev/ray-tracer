#include "triangle.h"
#include "rayintersection.h"

Triangle::Triangle(Vector vertex0, Vector vertex1, Vector vertex2, MaterialPointer material)
  : Shape(material),
    mVertex0(vertex0),
    mVertex1(vertex1),
    mVertex2(vertex2) {
  Vector e1 = vertex1 - vertex0;
  Vector e2 = vertex2 - vertex0;
  mNormal = e1.crossProduct(e2);
  mNormal.normalize();
}

Triangle::~Triangle() {
}

RayIntersection Triangle::intersectWithRay(const Ray &ray) const {
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

  TrianglePointer pointer = TrianglePointer(new Triangle(*this));
  return RayIntersection(true, pointer, f, getNormal(ray, f));
}

Vector Triangle::getNormal(const Ray &ray, float distance) const {
  return mNormal;
}