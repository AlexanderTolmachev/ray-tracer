#include "csgunionoperation.h"
#include "rayintersection.h"

CSGUnionOperation::CSGUnionOperation(CSGNodePointer leftArgument, CSGNodePointer rightArgument)
  : CSGBinaryOperationNode(leftArgument, rightArgument) {
}

CSGUnionOperation::~CSGUnionOperation() {
}

RayIntersection CSGUnionOperation::intersectWithRay(const Ray &ray) const {
  RayIntersection leftArgumentIntersection  = mLeftArgument->intersectWithRay(ray);
  RayIntersection rightArgumentIntersection = mRightArgument->intersectWithRay(ray);

  RayIntersection unionIntersection;
  unionIntersection.rayIntersectsWithShape = leftArgumentIntersection.rayIntersectsWithShape || rightArgumentIntersection.rayIntersectsWithShape;
  if (!unionIntersection.rayIntersectsWithShape) {
    return RayIntersection();
  }

  if (leftArgumentIntersection.distanceFromRayOrigin < rightArgumentIntersection.distanceFromRayOrigin) {
    unionIntersection.distanceFromRayOrigin = leftArgumentIntersection.distanceFromRayOrigin;
    unionIntersection.normalAtInresectionPoint = leftArgumentIntersection.normalAtInresectionPoint;
    unionIntersection.shape	= leftArgumentIntersection.shape;
  } else {
    unionIntersection.distanceFromRayOrigin = rightArgumentIntersection.distanceFromRayOrigin;
    unionIntersection.normalAtInresectionPoint = rightArgumentIntersection.normalAtInresectionPoint;
    unionIntersection.shape	= rightArgumentIntersection.shape;
  }

  // Copy intersection distances separately
  if (leftArgumentIntersection.rayIntersectsWithShape) {
    unionIntersection.intersectionDistances.resize(leftArgumentIntersection.intersectionDistances.size());
    std::copy(leftArgumentIntersection.intersectionDistances.begin(), leftArgumentIntersection.intersectionDistances.end(), unionIntersection.intersectionDistances.begin());
  }
  if (rightArgumentIntersection.rayIntersectsWithShape) {
    int size = unionIntersection.intersectionDistances.size();
    unionIntersection.intersectionDistances.resize(unionIntersection.intersectionDistances.size() + rightArgumentIntersection.intersectionDistances.size());
    std::copy(rightArgumentIntersection.intersectionDistances.begin(), rightArgumentIntersection.intersectionDistances.end(), unionIntersection.intersectionDistances.begin() + size);
  }

  return unionIntersection;
}

Vector CSGUnionOperation::getNormal(const Ray &ray, float distance) const {
  // This method is actually never called
  return Vector();
}
