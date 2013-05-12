/*!
 *\file types.h
 *\brief Contains common types definition
 */

#pragma once

#include <vmath.h>

#define FLOAT_ZERO 0.000001f
// Small value used to emit shadow rays
#define EPS 0.001f 
#define MAX_DISTANCE_TO_INTERSECTON 10000000000.0f

typedef Vector3f Vector;
typedef Vector3f Color;
typedef Vector2f Roots;