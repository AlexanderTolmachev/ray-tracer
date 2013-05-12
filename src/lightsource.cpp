/*!
 *\file lightsource.cpp
 *\brief Contains LightSource class definition
 */

#include <math.h>

#include "lightsource.h"
#include "scene.h"


LightSource::LightSource(Color ambientIntensity, Color diffuseIntensity, Color specularIntensity) 
  : mAmbientIntensity(ambientIntensity),
    mDiffuseIntensity(diffuseIntensity),
    mSpecularIntensity(specularIntensity) {
}

LightSource::~LightSource() {
}

///*
//* public:
//*/
//LightSource::LightSource() 
//  : constantAttenutaionCoefficient(0), 
//    linearAttenutaionCoefficient(0), 
//    quadraticAttenutaionCoefficient(0),
//    umbraAngle(0),
//    penumbraAngle(0),
//    falloffFactor(0),
//    range(0) {
//}

//Color LightSource::calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const {  
//  Vector point = ray.getPointAt(distance);
//  
//  Vector positionRelativeToSource = position - point;
//  float attenuation = calculateAttenuation(positionRelativeToSource);
//
//  Color ambientColor = material->ambientColor * ambientIntensity;
//
//  Vector shadowRayDirection;
//  if (type == DIRECTIONAL || type == SPOT) {
//    shadowRayDirection = -direction;
//  } else {
//    shadowRayDirection = positionRelativeToSource;
//  }
//
//  Ray shadowRay = Ray(point + shadowRayDirection * EPS, shadowRayDirection);
//  RayIntersection shadowRayIntersection = scene.calculateFirstIntersection(shadowRay);
//  if (shadowRayIntersection.rayIntersectsWithShape && shadowRayIntersection.distanceFromRayOrigin < positionRelativeToSource.length()) {
//    return ambientColor * attenuation;
//  }
//  
//  float cosineRayNormal = normal.dotProduct(shadowRay.getDirection());
//  if (cosineRayNormal <= 0) {
//    return ambientColor * attenuation;
//  }
//
//  Color diffuseColor = material->diffuseColor * diffuseIntensity * cosineRayNormal;
//  
//  Vector reflect = normal * 2 *shadowRay.getDirection().dotProduct(normal) - shadowRay.getDirection();
//  reflect.normalize();
//  Vector cameraDirection = ray.getOriginPosition() - point;
//  cameraDirection.normalize();
//
//  float cosineCameraDirectionReflect = cameraDirection.dotProduct(reflect);
//  if (cosineCameraDirectionReflect <= 0) {
//    return ambientColor * attenuation + diffuseColor * attenuation;
//  }
//
//  Color specularColor = material->specularColor * specularIntensity * powf(cosineCameraDirectionReflect, material->specularPower);
//
//  return ambientColor * attenuation + diffuseColor * attenuation + specularColor * attenuation;
//}
//
///*
//* private:
//*/
//float LightSource::calculateAttenuation(Vector position) const {
//  // Directional light has no attenuation
//  if (type == DIRECTIONAL) {
//    return 1.0;
//  }
//  
//  float distance = position.length();
//  float distanceAttenuation = 1.0 / (constantAttenutaionCoefficient + linearAttenutaionCoefficient * distance + quadraticAttenutaionCoefficient * distance * distance);
//
//  if (type == POINT) {
//    return distanceAttenuation;
//  }
//
//  Vector nirmalizedPosition = position;
//  nirmalizedPosition.normalize();
//  float angleCosine = nirmalizedPosition.dotProduct(-direction);
//  float cosineOfHalfUmbraAngle = cosf(umbraAngle / 2.0);
//  float cosineOfHalfPenumbraAngle = cosf(penumbraAngle / 2.0);
//
//  float spotAttenuation;
//  if (angleCosine > cosineOfHalfUmbraAngle) {
//    spotAttenuation = 1.0;  
//  } else if (angleCosine < cosineOfHalfPenumbraAngle) {
//    spotAttenuation = 0.0;
//  } else {
//    const float coeff = (angleCosine - cosineOfHalfPenumbraAngle) / (cosineOfHalfUmbraAngle - cosineOfHalfPenumbraAngle);
//    spotAttenuation = powf(coeff, falloffFactor);
//  }
//
//  return distanceAttenuation * spotAttenuation;
//}