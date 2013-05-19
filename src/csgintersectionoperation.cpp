#include "csgintersectionoperation.h"
#include "rayintersection.h"

CSGIntersectionOperation::CSGIntersectionOperation(CSGNodePointer leftArgument, CSGNodePointer rightArgument) 
 : CSGBinaryOperationNode(leftArgument, rightArgument) {
}

CSGIntersectionOperation::~CSGIntersectionOperation() {
}

RayIntersection CSGIntersectionOperation::intersectWithRay(const Ray &ray) const {
  RayIntersection leftArgumentIntersection  = mLeftArgument->intersectWithRay(ray);
  RayIntersection rightArgumentIntersection = mRightArgument->intersectWithRay(ray);
	
  // Ray should intersect both arguments
	if (!leftArgumentIntersection.rayIntersectsWithShape || !rightArgumentIntersection.rayIntersectsWithShape) {
		return RayIntersection();
	}

	float leftMin = MAX_DISTANCE_TO_INTERSECTON;
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

	RayIntersection intersection;
  intersection.rayIntersectsWithShape = true;
	if (leftMin < rightMin && leftMax > rightMin) {
		intersection.distanceFromRayOrigin = rightMin;
		intersection.shape = rightArgumentIntersection.shape;
		intersection.normalAtInresectionPoint = rightArgumentIntersection.normalAtInresectionPoint;
		intersection.intersectionDistances.push_back(rightMin);
		intersection.intersectionDistances.push_back(std::min(leftMax, rightMax));
	} else if (rightMin < leftMin && rightMax > leftMin) {
		intersection.distanceFromRayOrigin = leftMin;
		intersection.shape = leftArgumentIntersection.shape;
		intersection.normalAtInresectionPoint = leftArgumentIntersection.normalAtInresectionPoint;
		intersection.intersectionDistances.push_back(leftMin);
		intersection.intersectionDistances.push_back(std::min(leftMax, rightMax));
	} else {
		return RayIntersection();
	}
		
	return intersection;
}

Vector CSGIntersectionOperation::getNormal(const Ray &ray, float distance) const {
  // This method is actually never called
  return Vector();
}
