/*!
 *\file scene.cpp
 *\brief Contains Scene class definition
 */

#include "scene.h"

Scene::Scene() 
  : mBackgroundMaterial(NULL),
    mCamera(NULL) {
}

Scene::~Scene() {
}

void Scene::setCamera(CameraPointer camera) {
  mCamera = camera;
}

void Scene::addLightSource(LightSourcePointer lightSource) {
  mLightSources.push_back(lightSource);
}

void Scene::addShape(ShapePointer shape) {
  mShapes.push_back(shape);
}

void Scene::setBackgroundMaterial(MaterialPointer material) {
  mBackgroundMaterial = material;
}

CameraPointer Scene::getCamera() const {
  return mCamera;
}

MaterialPointer Scene::getBackgroundMaterial() const {
  return mBackgroundMaterial;
}

RayIntersection Scene::calculateNearestIntersection(const Ray &ray) const {
  RayIntersection nearestIntersection;

  for each (auto shape in mShapes) {
    RayIntersection intersection = shape->intersectWithRay(ray);
    if (!intersection.rayIntersectsWithShape) {
      continue;
    }

    if (intersection.distanceFromRayOrigin < nearestIntersection.distanceFromRayOrigin) {
      nearestIntersection = intersection;
    }
  }

  return nearestIntersection;
}

RayIntersection Scene::calculateFirstIntersection(const Ray &ray) const {
  for each (auto shape in mShapes) {
    RayIntersection intersection = shape->intersectWithRay(ray);
    if (intersection.rayIntersectsWithShape) {
      return intersection;
    }
  }

  return RayIntersection();
}

Color Scene::calculateIlluminationColor(const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const {
  Color illuminationColor;

  for each (auto lightSource in mLightSources) {
    illuminationColor += lightSource->calculateColor(*this, ray, distance, normal, material);
  }

  return illuminationColor;
}