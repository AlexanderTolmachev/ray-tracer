#include "pointlight.h"

PointLight::PointLight(Color ambientIntensity, Color diffuseIntensity, Color specularIntensity, 
                       Vector position, Vector direction, 
                       float constantAttenutaionCoefficient, float linearAttenutaionCoefficient, float quadraticAttenutaionCoefficient) 
  : LightSource(ambientIntensity, diffuseIntensity, specularIntensity),
    mPosition(position),
    mDirection(direction),
    mConstantAttenutaionCoefficient(constantAttenutaionCoefficient),
    mLinearAttenutaionCoefficient(linearAttenutaionCoefficient),
    mQuadraticAttenutaionCoefficient(quadraticAttenutaionCoefficient) {
}

PointLight::~PointLight() {
}

Color PointLight::calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const {
  return Color();
}