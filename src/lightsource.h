/*!
 *\file lightsource.h
 *\brief Contains LightSource class declaration
 */

#pragma once

#include <QSharedPointer>

#include "types.h"
#include "shape.h"
#include "ray.h"

struct LightSource;
class Scene;

typedef QSharedPointer<LightSource> LightSourcePointer;

enum LightSourceType
{
  POINT,
  DIRECTIONAL,
  SPOT
};

struct LightSource {
  LightSource();

  Color calculateColor(const Scene &scene, ShapePointer shape, const Ray &ray, float distance, const Vector &normal) const;

  // Type of the light source
  LightSourceType type;

  // Light position (for spot and point lights)
  Vector position;
  // Light direction (for spot and direction lights)
  Vector direction;
 
  // Colors intensity
  Color	ambientIntensity;
  Color	diffuseIntensity;
  Color specularIntensity;

  // Attenuation coefficients (for spot and point lights)
  float	constantAttenutaionCoefficient;
  float linearAttenutaionCoefficient;
  float quadraticAttenutaionCoefficient;

  // Umbra angle (for spot ligh)
  float	umbraAngle;
  // Penumbra angle (for spot light)
  float	penumbraAngle;
  // Falloff factor (for spot light)
  float	falloffFactor;

  // Light range (for directional light)
  float	range;
};