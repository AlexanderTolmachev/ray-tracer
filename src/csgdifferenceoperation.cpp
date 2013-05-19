#include "csgdifferenceoperation.h"
#include "rayintersection.h"

CSGDifferenceOperation::CSGDifferenceOperation(CSGNodePointer leftArgument, CSGNodePointer rightArgument) 
  : CSGBinaryOperationNode(leftArgument, rightArgument) {
}

CSGDifferenceOperation::~CSGDifferenceOperation() {
}

RayIntersection CSGDifferenceOperation::intersectWithRay(const Ray &ray) const {
  RayIntersection leftArgumentIntersection  = mLeftArgument->intersectWithRay(ray);
  RayIntersection rightArgumentIntersection = mRightArgument->intersectWithRay(ray);

  if (!leftArgumentIntersection.rayIntersectsWithShape) {
    return RayIntersection();
  }
  if (!rightArgumentIntersection.rayIntersectsWithShape) {
    return leftArgumentIntersection;
  }

  float leftMin  = MAX_DISTANCE_TO_INTERSECTON;
  float leftMax  = -1.f;
  float rightMin = MAX_DISTANCE_TO_INTERSECTON;
  float rightMax = -1.f;

  // Sort intersection distances to find min and max
  std::sort(leftArgumentIntersection.intersectionDistances.begin(), leftArgumentIntersection.intersectionDistances.end());
  std::sort(rightArgumentIntersection.intersectionDistances.begin(), rightArgumentIntersection.intersectionDistances.end());

  leftMin  = leftArgumentIntersection.intersectionDistances.front();  
  leftMax  = leftArgumentIntersection.intersectionDistances.back();
  rightMin = rightArgumentIntersection.intersectionDistances.front(); 
  rightMax = rightArgumentIntersection.intersectionDistances.back();

  RayIntersection differenceIntersection;
  differenceIntersection.rayIntersectsWithShape = true;

  // Right argument is closer, that left one
  if (rightMax < leftMin) {
    return leftArgumentIntersection;
  }

  // Left argument is closer than right one
  if (leftMax < rightMin) {
    return leftArgumentIntersection;
  }

  if (leftMin < rightMin)
  {
    differenceIntersection.distanceFromRayOrigin = leftMin;
    differenceIntersection.shape = leftArgumentIntersection.shape;
    differenceIntersection.normalAtInresectionPoint = leftArgumentIntersection.normalAtInresectionPoint; 
  } else if (rightMax < leftMax) {
    differenceIntersection.distanceFromRayOrigin = rightMax;
    differenceIntersection.shape = rightArgumentIntersection.shape;
    differenceIntersection.normalAtInresectionPoint = -rightArgumentIntersection.normalAtInresectionPoint;
  } else  {
    return RayIntersection();
  }

  // Copy intersection distances separately
  differenceIntersection.intersectionDistances.resize(leftArgumentIntersection.intersectionDistances.size() + rightArgumentIntersection.intersectionDistances.size());
  std::copy(leftArgumentIntersection.intersectionDistances.begin(), leftArgumentIntersection.intersectionDistances.end(), differenceIntersection.intersectionDistances.begin());
  std::copy(rightArgumentIntersection.intersectionDistances.begin(), rightArgumentIntersection.intersectionDistances.end(), differenceIntersection.intersectionDistances.begin() + leftArgumentIntersection.intersectionDistances.size());

  return differenceIntersection;
}

Vector CSGDifferenceOperation::getNormal(const Ray &ray, float distance) const {
  // This method is actually never called
  return Vector();
}
