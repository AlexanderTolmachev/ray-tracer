#pragma once

#include "lightsource.h"

class PointLight;

typedef QSharedPointer<PointLight> PointLightPointer;

class PointLight : public LightSource {
  public:
    PointLight(Color ambientIntensity, Color diffuseIntensity, Color specularIntensity, Vector position, 
               float constantAttenutaionCoefficient, float linearAttenutaionCoefficient, float quadraticAttenutaionCoefficient);
    ~PointLight();

    virtual Color calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const;

  private:
    // Light position
    Vector mPosition;

    // Attenuation coefficients
    float	mConstantAttenutaionCoefficient;
    float mLinearAttenutaionCoefficient;
    float mQuadraticAttenutaionCoefficient;
};