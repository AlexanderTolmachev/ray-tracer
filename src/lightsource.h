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

  // Attenuation coefficients
  float	constantAttenutaionCoefficient;
  float linearAttenutaionCoefficient;
  float quadraticAttenutaionCoefficient;

  // Penumbra angle in radians (for spot light)
  float	penumbraAngle;
  // Umbra angle in radians (for spot ligh)
  float	umbraAngle;
  // Falloff factor (for spotlight)
  float	falloffFactor;

  // Light range (for directional light)
  float	range;
};