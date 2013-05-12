/*!
 *\file cone.h
 *\brief Contains Cone class definition
 */

#include "cone.h"
#include "rayintersection.h"

Cone::Cone(Vector top, Vector bottomCenter, float radius, MaterialPointer material)
  : Shape(material),
    mTop(top),
    mBottomCenter(bottomCenter),
    mRadius(radius) {
}

Cone::~Cone() {
}

RayIntersection Cone::intersectWithRay(const Ray &ray) const {
  Vector coneAxis	= (mBottomCenter - mTop);
  coneAxis.normalize();

  Vector rayOriginPosition = ray.getOriginPosition();
  Vector rayDirection = ray.getDirection();

  Vector bottomCenterToRayOrigin = rayOriginPosition - mTop;

  float rayDirectionDotAxis = rayDirection.dotProduct(coneAxis);
  float bottomCenterToRayOriginDotAxis = bottomCenterToRayOrigin.dotProduct(coneAxis);
  float radiansPerHeight = mRadius / (mBottomCenter - mTop).length();

  Vector u = rayDirection + coneAxis * (-rayDirectionDotAxis);
  Vector v = bottomCenterToRayOrigin + coneAxis * (-bottomCenterToRayOriginDotAxis);
  float  w = bottomCenterToRayOriginDotAxis * radiansPerHeight;

  float	radiansPerDirection = rayDirectionDotAxis * radiansPerHeight;


  // Solve square equation a * x^2 + b * x + c = 0
  float a = u.dotProduct(u) - radiansPerDirection * radiansPerDirection;
  float closestRoot = -1.f;
  float root		= 0.f;
  
  if (fabs(a) > FLOAT_ZERO) {
    float b = 2 * (u.dotProduct(v) - w * radiansPerDirection);
    float c = v.dotProduct(v) - w * w;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0) {
      return RayIntersection();
    }

    discriminant = sqrtf(discriminant);
    float denominator = 1.0 / (2.0 * a);

    root = (-b - discriminant) * denominator;
    if (root > 0.0) {
      Vector point = ray.getPointAt(root);
      Vector bottomCenterToPoint = point - mTop;
      Vector topToPoint = point - mBottomCenter;
      if (coneAxis.dotProduct(bottomCenterToPoint) > 0.0 && (-coneAxis).dotProduct(topToPoint) > 0.0) {
        closestRoot = root;
      }
    }
    
    root = (-b + discriminant) * denominator;
    if (root > 0.0) {
      Vector point = ray.getPointAt(root);
      Vector bottomCenterToPoint = point - mTop;
      Vector topToPoint = point - mBottomCenter;
      if (coneAxis.dotProduct(bottomCenterToPoint) > 0.0 && (-coneAxis).dotProduct(topToPoint) > 0.0) {
        if (closestRoot < 0.0) {
          closestRoot = root;
        } else if (root < closestRoot) {
          closestRoot = root;
        }
      }
    }
  }

  // Intersection with bottom
  if (fabs(rayDirectionDotAxis) < FLOAT_ZERO) {
    if (closestRoot > 0.0) {
      ConePointer pointer = ConePointer(new Cone(*this));
      return RayIntersection(true, pointer, closestRoot, getNormal(ray, closestRoot));
    }

    return RayIntersection();
  }

  // Intersection with top and bottom points
  root = (-coneAxis).dotProduct(rayOriginPosition - mBottomCenter) / rayDirectionDotAxis;
  if (root > 0.0)
  {
    Vector topToPoint = ray.getPointAt(root) - mBottomCenter;
    if (topToPoint.dotProduct(topToPoint) < mRadius * mRadius)
    {
      if (closestRoot < 0.0) {
        closestRoot = root;
      } else if (root < closestRoot) {
        closestRoot = root;
      }
    }
  }

  if (closestRoot > 0.0) {
    ConePointer pointer = ConePointer(new Cone(*this));
    return RayIntersection(true, pointer, closestRoot, getNormal(ray, closestRoot));
  }

  return RayIntersection();  
}

Vector Cone::getNormal(const Ray &ray, float distance, const RayIntersection &intersection) const {
  Vector coneAxis	= (mBottomCenter - mTop);
  coneAxis.normalize();
  Vector point = ray.getPointAt(distance);

  // If point is lying at bottom
  Vector pointPositionRelativelyToBottom	= point - mBottomCenter;
  if (fabs(coneAxis.dotProduct(pointPositionRelativelyToBottom)) < FLOAT_ZERO && 
      pointPositionRelativelyToBottom.dotProduct(pointPositionRelativelyToBottom) < mRadius * mRadius) {
    return coneAxis;
  }

  // Otherwise, if point is lying at side surface
  Vector approximatedNormal = point - (coneAxis * (point - mBottomCenter).dotProduct(coneAxis) + mBottomCenter);
  float radiansPerHeight = mRadius / (mBottomCenter - mTop).length();
  Vector normal = approximatedNormal + coneAxis * (-radiansPerHeight * approximatedNormal.length());
  normal.normalize();
  return normal;
}