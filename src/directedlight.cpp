#include "directedlight.h"

DirectedLight::DirectedLight(Color ambientIntensity, Color diffuseIntensity, Color specularIntensity, 
                             Vector direction,  float range) 
  : LightSource(ambientIntensity, diffuseIntensity, specularIntensity),
    mDirection(direction),
    mRange(range) {
}

DirectedLight::~DirectedLight() {
}

Color DirectedLight::calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const {
  return Color();
}