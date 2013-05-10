#pragma once

#include "lightsource.h"

class SpotLight;

typedef QSharedPointer<SpotLight> SpotLightPointer;

class SpotLight : public LightSource {
public:
  SpotLight(Color ambientIntensity, Color diffuseIntensity, Color specularIntensity, 
            Vector position, Vector direction, 
            float constantAttenutaionCoefficient, float linearAttenutaionCoefficient, float quadraticAttenutaionCoefficient,
            float umbraAngle, float penumbraAngle, float falloffFactor);
  virtual ~SpotLight();

  virtual Color calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const;

private:
  // Light position
  Vector mPosition;
  // Light direction
  Vector mDirection;

  // Attenuation coefficients
  float	mConstantAttenutaionCoefficient;
  float mLinearAttenutaionCoefficient;
  float mQuadraticAttenutaionCoefficient;

  // Umbra angle
  float	mUmbraAngle;
  // Penumbra angle
  float	mPenumbraAngle;
  // Falloff factor
  float	mFalloffFactor;
};