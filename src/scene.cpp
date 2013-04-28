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