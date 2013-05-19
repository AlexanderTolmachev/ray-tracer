#include "box.h"
#include "rayintersection.h"

#define EPS_FOR_NORMAL_DIRECTION 0.0005f

Box::Box(Vector min, Vector max, MaterialPointer material)
  : Shape(material),
    mMin(min),
    mMax(max) {
}

Box::~Box() {
}

RayIntersection Box::intersectWithRay(const Ray &ray) const {
  Vector rayOriginPosition    = ray.getOriginPosition();
  Vector rayDirection = ray.getDirection();

  float tmax = MAX_DISTANCE_TO_INTERSECTON;
  float tmin = -MAX_DISTANCE_TO_INTERSECTON;

  if (fabs(rayDirection.x) < FLOAT_ZERO) {
    if (rayOriginPosition.x < mMin.x || rayOriginPosition.x > mMax.x) {
      return RayIntersection();
    }
  }

  if (fabs(rayDirection.y) < FLOAT_ZERO) {
    if (rayOriginPosition.y < mMin.y || rayOriginPosition.y > mMax.y) {
      return RayIntersection();
    }
  }

  if (fabs(rayDirection.z) < FLOAT_ZERO) {
    if (rayOriginPosition.z < mMin.z || rayOriginPosition.z > mMax.z) {
      return RayIntersection();
    }
  }

  float invDirection = 0.f;
  if (fabs(rayDirection.x) > FLOAT_ZERO) {
    invDirection = 1.f / rayDirection.x;
    tmin = (mMin.x - rayOriginPosition.x) * invDirection;
    tmax = (mMax.x - rayOriginPosition.x) * invDirection;

    if (tmin > tmax) {
      std::swap(tmin, tmax);
    }
  }

  if (fabs(rayDirection.y) > FLOAT_ZERO) {
    invDirection = 1.f / rayDirection.y;
    float tymin = (mMin.y - rayOriginPosition.y) * invDirection;
    float tymax = (mMax.y - rayOriginPosition.y) * invDirection;

    if (tymin > tymax) {
      std::swap(tymin, tymax);
    }

    if (tymin > tmin) {
      tmin = tymin;
    }
    if (tymax < tmax) {
      tmax = tymax;
    }
  }

  if (fabs(rayDirection.z) > FLOAT_ZERO) {
    invDirection = 1.f / rayDirection.z;
    float tzmin = (mMin.z - rayOriginPosition.z) * invDirection;
    float tzmax = (mMax.z - rayOriginPosition.z) * invDirection;

    if (tzmin > tzmax) {
      std::swap(tzmin, tzmax);
    }

    if (tzmin > tmin) {
      tmin = tzmin;
    }
    if (tzmax < tmax) {
      tmax = tzmax;
    }

  }

  if (!(tmin > 0 && tmax > tmin)) {
    return RayIntersection();
  }

  BoxPointer pointer = BoxPointer(new Box(*this));
  std::vector<float> intersectionDistances;
  intersectionDistances.resize(2);
  intersectionDistances[0] = tmin;
  intersectionDistances[1] = tmax;
  return RayIntersection(true, pointer, tmin, getNormal(ray, tmin), intersectionDistances);
}

Vector Box::getNormal(const Ray &ray, float distance) const {
  Vector point = ray.getPointAt(distance);
 
  Vector pointToMin = point - mMin;
  Vector pointToMax = point - mMax;

  if (fabs(pointToMin.x) < EPS_FOR_NORMAL_DIRECTION) {
    return Vector(-1.0, 0.0, 0.0);
  }
  if (fabs(pointToMax.x) < EPS_FOR_NORMAL_DIRECTION) {
    return Vector(1.0, 0.0, 0.0);
  }
  if (fabs(pointToMin.y) < EPS_FOR_NORMAL_DIRECTION) {
    return Vector(0.0, -1.0, 0.0);
  }
  if (fabs(pointToMax.y) < EPS_FOR_NORMAL_DIRECTION) {
    return Vector(0.0, 1.0, 0.0);
  }
  if (fabs(pointToMin.z) < EPS_FOR_NORMAL_DIRECTION) {
    return Vector(0.0, 0.0, -1.0);
  }
  if (fabs(pointToMax.z) < EPS_FOR_NORMAL_DIRECTION) {
    return Vector(0.0, 0.0, 1.0);
  }

  // This should never happen
  return Vector(0.0, 0.0, 0.0);
}
