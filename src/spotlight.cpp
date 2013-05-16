#include "spotlight.h"
#include "mathcommons.h"
#include "scene.h"

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
  mUmbraAngle = mUmbraAngle * M_PI / 180.0;
  mPenumbraAngle = mPenumbraAngle * M_PI / 180.0;
  mHalfUmbraAngleCosine = cosf(mUmbraAngle / 2.0);
  mHalfPenumbraAngleCosine = cosf(mPenumbraAngle / 2.0);
}

SpotLight::~SpotLight() {
}

Color SpotLight::calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const {
  Vector point = ray.getPointAt(distance);

  Color ambientComponent;
  Color diffuseComponent;
  Color specularComponent;

  ambientComponent = componentwiseProduct(material->ambientColor, mAmbientIntensity);
  Color result = ambientComponent;

  Vector lightVector = -mDirection;
  float	lightVectorDotNormal = lightVector.dotProduct(normal);

  Vector lightDirection = mPosition - point;
  lightDirection.normalize();

  float distanceToLight	= (mPosition - point).length();
  float distanceAttenuation = 1 / (mConstantAttenutaionCoefficient + mLinearAttenutaionCoefficient * distanceToLight + mQuadraticAttenutaionCoefficient * distanceToLight * distanceToLight);
  result *= distanceAttenuation;

  float lightDirectionDotLightVector = lightDirection.dotProduct(lightVector);
  float spotAttenuation = 0.0;
  if (lightDirectionDotLightVector > mHalfUmbraAngleCosine) {
    spotAttenuation = 1.0;
  } else if (lightDirectionDotLightVector < mHalfPenumbraAngleCosine) {
    spotAttenuation = 0.0;
  } else {
    const float factor = (lightDirectionDotLightVector - mHalfPenumbraAngleCosine) / (mHalfUmbraAngleCosine - mHalfPenumbraAngleCosine);
    spotAttenuation = powf(factor, mFalloffFactor);
  }

  result *= spotAttenuation;

  // If the point is not illuminated  
  if (lightVectorDotNormal <= 0.0) {
    return result;
  }

  if (lightDirectionDotLightVector <= 0.0) {
    return result;
  }

  Ray shadowRay(point + lightVector * EPS_FOR_SHADOW_RAYS, lightVector);	
  RayIntersection shadowRayIntersection = scene.calculateFirstIntersection(shadowRay);

  // Object not in the shadow
  if (!shadowRayIntersection.rayIntersectsWithShape || shadowRayIntersection.distanceFromRayOrigin > distanceToLight) {
    Color diffuseColor = material->diffuseColor;
    diffuseComponent = componentwiseProduct(diffuseColor, mDiffuseIntensity * spotAttenuation * distanceAttenuation * lightVectorDotNormal);

    Vector lightReflect = mDirection -  normal * 2 * mDirection.dotProduct(normal);
    lightReflect.normalize();

    Vector cameraDirection = ray.getOriginPosition() - point;
    cameraDirection.normalize();

    float	cosLightReflect = cameraDirection.dotProduct(lightReflect);
    if (cosLightReflect > 0.0) {
      Color specularColor = material->specularColor;
      specularComponent	= componentwiseProduct(specularColor, mSpecularIntensity * powf(cosLightReflect, material->specularPower) * spotAttenuation * distanceAttenuation);
    }				
  }

  result += (diffuseComponent + specularComponent);
  return result;
}