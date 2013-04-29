/*!
 *\file scene.h
 *\brief Contains Scene class declaration
 */

#pragma once

#include <QSharedPointer>
#include <vector>

#include "shape.h"
#include "lightsource.h"
#include "material.h"
#include "camera.h"
#include "rayintersection.h"

class Scene;

typedef QSharedPointer<Scene> ScenePointer;

class Scene {
  public:
    Scene();
    virtual ~Scene();

    void setCamera(CameraPointer camera);
    void addLightSource(LightSourcePointer lightSource);
    void addShape(ShapePointer shape);
    void setBackgroundMaterial(MaterialPointer material);

    CameraPointer getCamera() const;
    MaterialPointer getBackgroundMaterial() const;

    RayIntersection calculateNearestIntersection(const Ray &ray) const;
    RayIntersection calculateFirstIntersection(const Ray &ray) const;
    Color calculateIlluminationColor(const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const;

  private:
    CameraPointer mCamera;
    std::vector<LightSourcePointer> mLightSources;
    std::vector<ShapePointer> mShapes;
    MaterialPointer mBackgroundMaterial;
};
