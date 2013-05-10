#include "directedlight.h"

DirectedLight::DirectedLight(Color ambientIntensity, Color diffuseIntensity, Color specularIntensity, float range) 
  : LightSource(ambientIntensity, diffuseIntensity, specularIntensity),
    mRange(range) {
}

DirectedLight::~DirectedLight() {
}

Color DirectedLight::calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const {
  return Color();
}