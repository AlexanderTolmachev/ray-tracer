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
  shadowRayDirection /= distanceToLight; 
  float	shadowRayDotNormal = shadowRayDirection.dotProduct(normal);
  result *= attenuation;

  // If the point is not illuminated
  if (shadowRayDotNormal <= 0.0) {
    return material->ambientColor;
  }

  Ray shadowRay(point + shadowRayDirection * EPS, shadowRayDirection);	
  RayIntersection shadowRayIntersection = scene.calculateFirstIntersection(shadowRay);
  
  // If object is not in shadow
  if (!shadowRayIntersection.rayIntersectsWithShape) {
    Color diffuseColor = material->diffuseColor;
    diffuseComponent  = componentwiseProduct(diffuseColor, mDiffuseIntensity * attenuation * shadowRayDotNormal);

    Vector lightReflect = shadowRayDirection - normal * 2 * shadowRayDirection.dotProduct(normal);
    lightReflect.normalize();

    Vector cameraDirection = ray.getOriginPosition() - point;
    cameraDirection.normalize();

    float	cosLightReflect = cameraDirection.dotProduct(lightReflect);
    if (cosLightReflect > 0.0) {
      const Color specularColor = material->specularColor; 
      specularComponent	= componentwiseProduct(specularColor, mSpecularIntensity * powf(cosLightReflect, material->specularPower) * attenuation);
    }				
  }

  result += (diffuseComponent + specularComponent);
  return result;
}