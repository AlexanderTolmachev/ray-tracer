/*!
 *\file types.h
 *\brief Contains common types definition
 */

#pragma once

#include <vmath.h>

#define FLOAT_ZERO 0.000001f
// Small value used to emit shadow rays
#define EPS_FOR_SHADOW_RAYS 0.01f
// Small value used to emit reflection rays
#define EPS_FOR_REFLECTION_RAYS 0.0001f

#define MAX_DISTANCE_TO_INTERSECTON FLT_MAX

typedef Vector3f Vector;
typedef Vector3f Color;
typedef Vector2f Roots;