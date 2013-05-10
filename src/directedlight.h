#pragma once

#include "lightsource.h"

class DirectedLight;

typedef QSharedPointer<DirectedLight> DirectedLightPointer;

class DirectedLight : public LightSource {
  public:
    DirectedLight(Color ambientIntensity, Color diffuseIntensity, Color specularIntensity, Vector direction, float range);
    virtual ~DirectedLight();

    virtual Color calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const;

  private:
    // Light direction
    Vector mDirection;
    // Light range
    float	mRange;
};