/*!
 *\file material.h
 *\brief Contains Material structure declaration
 */

#pragma once

#include <QSharedPointer>

#include "types.h"

struct Material;

typedef QSharedPointer<Material> MaterialPointer;

struct Material { 
  Material() : specularPower(0), densityFactor(0), illuminationFactor(0), reflectionFactor(0), refractionFactor(0) {}

  // Color properties as described by Phong model
  Color ambientColor;
  Color diffuseColor;
  Color specularColor;
  Color emissiveColor;

  float specularPower;
  float	densityFactor;

  // Illumination factors
  float	illuminationFactor;
  float	reflectionFactor;
  float	refractionFactor;
};