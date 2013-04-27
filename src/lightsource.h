/*!
 *\file lightsource.h
 *\brief Contains LightSource class declaration
 */

#pragma once

#include <QSharedPointer>

struct LightSource;

typedef QSharedPointer<LightSource> LightSourcePointer;

enum LightSourceType
{
  POINT,
  DIRECTIONAL,
  SPOT
};

struct LightSource {

};