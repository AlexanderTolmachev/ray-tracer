#include "pointlight.h"
#include "mathcommons.h"
#include "scene.h"

PointLight::PointLight(Color ambientIntensity, Color diffuseIntensity, Color specularIntensity, Vector position, 
                       float constantAttenutaionCoefficient, float linearAttenutaionCoefficient, float quadraticAttenutaionCoefficient) 
  : LightSource(ambientIntensity, diffuseIntensity, specularIntensity),
    mPosition(position),
    mConstantAttenutaionCoefficient(constantAttenutaionCoefficient),
    mLinearAttenutaionCoefficient(linearAttenutaionCoefficient),
    mQuadraticAttenutaionCoefficient(quadraticAttenutaionCoefficient) {
}

PointLight::~PointLight() {
}

Color PointLight::calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const {
  Vector point = ray.getPointAt(distance);

  Color ambientComponent;
  Color diffuseComponent;
  Color specularComponent;

  ambientComponent = componentwiseProduct(material->ambientColor, mAmbientIntensity);
  Color result = ambientComponent;

  Vector shadowRayDirection = mPosition - point;
  float	distanceToLight	= shadowRayDirection.length();
  float	attenuation	= 1 / (mConstantAttenutaionCoefficient + mLinearAttenutaionCoefficient * distanceToLight + mQuadraticAttenutaionCoefficient * distanceToLight * distanceToLight);
  result *= attenuation;

  shadowRayDirection.normalize(); 
  float	shadowRayDotNormal = shadowRayDirection.dotProduct(normal);

  // If the point is not illuminated
  if (shadowRayDotNormal <= 0.0) {
    return result;
  }

  Ray shadowRay(point + shadowRayDirection * EPS_FOR_SHADOW_RAYS, shadowRayDirection);	
  RayIntersection shadowRayIntersection = scene.calculateNearestIntersection(shadowRay);
  
  // If object is not in shadow
  if (!shadowRayIntersection.rayIntersectsWithShape || shadowRayIntersection.distanceFromRayOrigin > distanceToLight) {
    Color diffuseColor = material->diffuseColor;
    diffuseComponent  = componentwiseProduct(diffuseColor, mDiffuseIntensity * attenuation * shadowRayDotNormal);

    Vector lightReflect = shadowRayDirection - normal * 2 * shadowRayDirection.dotProduct(normal);
    lightReflect.normalize();

    Vector cameraDirection = ray.getOriginPosition() - point;
    cameraDirection.normalize();

    float	cosLightReflect = cameraDirection.dotProduct(lightReflect);
    if (cosLightReflect > 0.0) {
      Color specularColor = material->specularColor; 
      specularComponent	= componentwiseProduct(specularColor, mSpecularIntensity * powf(cosLightReflect, material->specularPower) * attenuation);
    }				
  }

  result += (diffuseComponent + specularComponent);
  return result;
}