#include "spotlight.h"
#include "mathcommons.h"

SpotLight::SpotLight(Color ambientIntensity, Color diffuseIntensity, Color specularIntensity, 
                     Vector position, Vector direction, 
                     float constantAttenutaionCoefficient, float linearAttenutaionCoefficient, float quadraticAttenutaionCoefficient,
                     float umbraAngle, float penumbraAngle, float falloffFactor)
  : LightSource(ambientIntensity, diffuseIntensity, specularIntensity),
  mPosition(position),
  mDirection(direction),
  mConstantAttenutaionCoefficient(constantAttenutaionCoefficient),
  mLinearAttenutaionCoefficient(linearAttenutaionCoefficient),
  mQuadraticAttenutaionCoefficient(quadraticAttenutaionCoefficient),
  mUmbraAngle(umbraAngle), 
  mPenumbraAngle(penumbraAngle), 
  mFalloffFactor(falloffFactor) {
}

SpotLight::~SpotLight() {
}

Color SpotLight::calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const {
  return Color();
}