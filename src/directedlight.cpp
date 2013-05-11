#include "directedlight.h"
#include "mathcommons.h"
#include "scene.h"

DirectedLight::DirectedLight(Color ambientIntensity, Color diffuseIntensity, Color specularIntensity, 
                             Vector direction,  float range) 
  : LightSource(ambientIntensity, diffuseIntensity, specularIntensity),
    mDirection(direction),
    mRange(range) {
}

DirectedLight::~DirectedLight() {
}

Color DirectedLight::calculateColor(const Scene &scene, const Ray &ray, float distance, const Vector &normal, MaterialPointer material) const {
  Vector point = ray.getPointAt(distance);

  Color ambientComponent;
  Color diffuseComponent;
  Color specularComponent;

  ambientComponent = componentwiseProduct(material->ambientColor, mAmbientIntensity);

  Vector lightVector = -mDirection;
  const float	lightVectorDotNormal	= lightVector.dotProduct(normal);  
  // If the point is not illuminated
  if (lightVectorDotNormal <= 0.0) {
    return ambientComponent;
  }

  Ray shadowRay(point + lightVector * EPS, lightVector);	
  RayIntersection shadowRayIntersection = scene.calculateFirstIntersection(shadowRay);
  // If object is not in shadow
  if (!shadowRayIntersection.rayIntersectsWithShape) {
    Color diffuseColor = material->diffuseColor;
    diffuseComponent = componentwiseProduct(diffuseColor, mDiffuseIntensity * lightVectorDotNormal);

    Vector lightReflect = mDirection - normal * 2 * mDirection.dotProduct(normal);
    lightReflect.normalize();

    Vector cameraDirection = ray.getOriginPosition() - point;
    cameraDirection.normalize();

    float	cameraDirectionDotLightReflect = cameraDirection.dotProduct(lightReflect);
    if (cameraDirectionDotLightReflect > 0.0) {
      const Color specularColor = material->specularColor; 
      specularComponent	= componentwiseProduct(specularColor, mSpecularIntensity * powf(cameraDirectionDotLightReflect, material->specularPower));
    }				
  }
  
  return ambientComponent + diffuseComponent + specularComponent;
}