/*!
 *\file lightsource.h
 *\brief Contains LightSource class declaration
 */

#pragma once

#include <QSharedPointer>

#include "types.h"
#include "shape.h"
#include "ray.h"

struct LightSource;
class Scene;

typedef QSharedPointer<LightSource> LightSourcePointer;

struct LightSource {
  public:
    LightSource(Color ambientIntensity, Color diffuseIntensity, Color specularIntensity);
    virtual ~LightSource();

    virtual Color calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const = 0;

  protected:
    // Colors intensity
    Color	mAmbientIntensity;
    Color	mDiffuseIntensity;
    Color mSpecularIntensity;
};