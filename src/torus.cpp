#include <quarticsolver.h>

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
  Vector rayOriginPosition = ray.getOriginPosition();
  Vector rayDirection = ray.getDirection();
  
  Vector centerToRayOrigin = rayOriginPosition - mCenter;
  const float centerToRayOriginDotDirection = rayDirection.dotProduct(centerToRayOrigin);
  float	centerToRayOriginDotDirectionSquared = centerToRayOrigin.dotProduct(centerToRayOrigin);
  float innerRadiusSquared = mInnerRadius * mInnerRadius;
  float outerRadiusSquared = mOuterRadius * mOuterRadius;

  float	axisDotCenterToRayOrigin	= mAxis.dotProduct(centerToRayOrigin);
  float	axisDotRayDirection = mAxis.dotProduct(rayDirection);
  float	a = 1 - axisDotRayDirection * axisDotRayDirection;
  float	b = 2 * (centerToRayOrigin.dotProduct(rayDirection) - axisDotCenterToRayOrigin * axisDotRayDirection);
  float c = centerToRayOriginDotDirectionSquared - axisDotCenterToRayOrigin * axisDotCenterToRayOrigin;
  float	d = centerToRayOriginDotDirectionSquared + outerRadiusSquared - innerRadiusSquared;

  // Solve quartic equation with coefficients A, B, C, D and E
  float A = 1; 
  float B = 4 * centerToRayOriginDotDirection;
  float C = 2 * d + B * B * 0.25f - 4 * outerRadiusSquared * a;
  float D = B * d - 4 * outerRadiusSquared * b;
  float E = d * d - 4 * outerRadiusSquared * c;
  
  // Maximum number of roots is 4
  QuarticEquation equation(A, B, C, D, E);
  const int maxRootsCount = 4;
  double roots[maxRootsCount] = {-1.0, -1.0, -1.0, -1.0};
  int rootsCount = equation.Solve(roots);

  if (rootsCount == 0) {
    return RayIntersection();
  }

  // Find closest to zero positive solution
  float closestRoot = MAX_DISTANCE_TO_INTERSECTON;
  std::vector<float> intersectionDistances;  
  for (int idx = 0; idx < maxRootsCount; ++idx) {
    float root = roots[idx];
    if (root > FLOAT_ZERO && root < closestRoot) {
      closestRoot = root;
      intersectionDistances.push_back(root);
    }
  }

  if (closestRoot != MAX_DISTANCE_TO_INTERSECTON) {
    std::sort(intersectionDistances.begin(), intersectionDistances.end());
    TorusPointer pointer = TorusPointer(new Torus(*this));
    return RayIntersection(true, pointer, closestRoot, getNormal(ray, closestRoot), intersectionDistances);
  }

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
