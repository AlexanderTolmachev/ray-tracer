/*!
 *\file cylinder.cpp
 *\brief Contains Cylinder class definition
 */

#include "cylinder.h"
#include "rayintersection.h"

Cylinder::Cylinder(Vector topCenter, Vector bottomCenter, float radius, MaterialPointer material) 
  : Shape(material),
    mTopCenter(topCenter),
    mBottomCenter(bottomCenter),
    mRadius(radius) {
}

Cylinder::~Cylinder() {
}

RayIntersection Cylinder::intersectWithRay(const Ray &ray) const {
  Vector cylinderAxis = mTopCenter - mBottomCenter;
  cylinderAxis.normalize();

  Vector rayOrigin    = ray.getOriginPosition();
  Vector rayDirection = ray.getDirection();

  Vector bottomCenterToRayOrigin = rayOrigin - mBottomCenter;

  Vector u = rayDirection - cylinderAxis * rayDirection.dotProduct(cylinderAxis);
  Vector v = bottomCenterToRayOrigin - cylinderAxis * bottomCenterToRayOrigin.dotProduct(cylinderAxis);

  // Solve square equation a * x^2 + b * x + c = 0
  float a = u.dotProduct(u);
  float root		= 0.0;
  float closestRoot = -1.0;
  if (fabs(a) > FLOAT_ZERO) {
    const float b = 2 * u.dotProduct(v);
    const float c = v.dotProduct(v) - mRadius * mRadius;

    float descriminant = b * b - 4 * a * c;

    if (descriminant < 0.0) {
      return RayIntersection();
    }

    descriminant = sqrtf(descriminant);

    // Calculate roots and take closest
    float denominator = 1.0 / (2.0 * a);

    root = (-b - descriminant) * denominator;
    if (root >= 0.0) {
      Vector point = ray.getPointAt(root);    
      Vector pointRelativelyToBottomCenter = point - mBottomCenter;
      Vector pointRelativelyToTopCenter		= point - mTopCenter;
      if (cylinderAxis.dotProduct(pointRelativelyToBottomCenter) > 0.0 && 
          cylinderAxis.dotProduct(pointRelativelyToTopCenter) < 0.0) {
        closestRoot = root;
      }
    }
    
    root = (-b + descriminant) * denominator;
    if (root > 0.0) {
      Vector point = ray.getPointAt(root);    
      Vector pointRelativelyToBottomCenter = point - mBottomCenter;
      Vector pointRelativelyToTopCenter		= point - mTopCenter;
      if (cylinderAxis.dotProduct(pointRelativelyToBottomCenter) > 0.0 && 
          cylinderAxis.dotProduct(pointRelativelyToTopCenter) < 0.0) {
        if (closestRoot < 0.0) {
          root = closestRoot;
        } else if (root < closestRoot) {
          closestRoot = root;
        }
      }
    }
  }

  // Find intersection with bottom
  float cyliderAxisDotDirection = cylinderAxis.dotProduct(rayDirection);
  if (fabs(cyliderAxisDotDirection) < FLOAT_ZERO) {
    if (closestRoot > 0.f) {
      CylinderPointer pointer = CylinderPointer(new Cylinder(*this));
      return RayIntersection(true, pointer, closestRoot, getNormal(ray, closestRoot));
    }

    return RayIntersection();
  }

  float cylinderAxisDotRayOrigin = cylinderAxis.dotProduct(rayOrigin);
  float bottomCenterToRayOriginDotCylinderAxis = bottomCenterToRayOrigin.dotProduct(cylinderAxis);
  root = -bottomCenterToRayOriginDotCylinderAxis / cyliderAxisDotDirection;
  if (root > 0.0) {
    Vector point = ray.getPointAt(root);    
    Vector pointRelativelyToBottomCenter = point - mBottomCenter;
    if (pointRelativelyToBottomCenter.dotProduct(pointRelativelyToBottomCenter) < mRadius * mRadius) {
      if (closestRoot < 0.0) {
        closestRoot = root;
      } else if (root < closestRoot) {
        closestRoot = root;
      }
    }
  }

  // Find intersection with top
  Vector topCenterToRayOrigin = rayOrigin - mTopCenter;
  float topCenterToRayOriginDotAxis = topCenterToRayOrigin.dotProduct(-cylinderAxis);
  root = topCenterToRayOriginDotAxis / cyliderAxisDotDirection;
  if (root > 0.0) {
    Vector point = ray.getPointAt(root);    
    Vector pointRelativelyToTopCenter = point - mTopCenter;
    if (pointRelativelyToTopCenter.dotProduct(pointRelativelyToTopCenter) < mRadius * mRadius) {
      if (closestRoot < 0.0) {
        closestRoot = root;
      } else if (root < closestRoot) {
        closestRoot = root;
      } 
    }
  }

  if (closestRoot >= 0.0) {
    CylinderPointer pointer = CylinderPointer(new Cylinder(*this));
    return RayIntersection(true, pointer, closestRoot, getNormal(ray, closestRoot));
  }
  
  return RayIntersection();
}

Vector Cylinder::getNormal(const Ray &ray, float distance) const {
  Vector intersectionPoint = ray.getPointAt(distance);
  Vector cylinderAxis = mTopCenter - mBottomCenter;
  cylinderAxis.normalize();
  float squaredRadius = mRadius * mRadius;

  // Check if intersection point is at cylinder top surface
  Vector intersectionPointRelativelyToTopCenter = intersectionPoint - mTopCenter;
  if (cylinderAxis.dotProduct(intersectionPointRelativelyToTopCenter) < FLOAT_ZERO &&
      intersectionPointRelativelyToTopCenter.dotProduct(intersectionPointRelativelyToTopCenter) < squaredRadius) {
    return cylinderAxis;
  }

  // Check if intersection point is at cylinder bottom surface
  Vector intersectionPointRelativelyToBottomCenter = intersectionPoint - mBottomCenter;
  if (cylinderAxis.dotProduct(intersectionPointRelativelyToBottomCenter) < FLOAT_ZERO &&
      intersectionPointRelativelyToBottomCenter.dotProduct(intersectionPointRelativelyToBottomCenter) < squaredRadius) {
    return -cylinderAxis;
  }

  // Otherwise, if intersection point is at cylinder side surface
  Vector normal = intersectionPoint - cylinderAxis * intersectionPointRelativelyToBottomCenter.dotProduct(cylinderAxis) - mBottomCenter;
  normal.normalize();
  return normal;
}