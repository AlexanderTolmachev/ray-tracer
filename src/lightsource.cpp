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