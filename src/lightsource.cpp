/*!
 *\file lightsource.cpp
 *\brief Contains LightSource class definition
 */

#include "lightsource.h"
#include "scene.h"

LightSource::LightSource() 
  : constantAttenutaionCoefficient(0), 
    linearAttenutaionCoefficient(0), 
    quadraticAttenutaionCoefficient(0),
    umbraAngle(0),
    penumbraAngle(0),
    falloffFactor(0),
    range(0) {
}

Color LightSource::calculateColor(const Scene &scene, ShapePointer shape, const Ray &ray, float distance, const Vector &normal) const {
  Color color;
  // TODO implement
  return color;
}