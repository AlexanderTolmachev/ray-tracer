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

class Scene;

typedef QSharedPointer<Scene> ScenePointer;

class Scene {
  public:
    Scene();
    virtual ~Scene();



  private:
    std::vector<ShapePointer> mShapes;
    std::vector<LightSourcePointer> mLightSources;
    MaterialPointer mBackgroundMaterial;
    CameraPointer mCamera;
};
