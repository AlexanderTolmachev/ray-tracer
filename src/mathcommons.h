#pragma once

#include "types.h"

inline Vector componentwiseProduct(const Vector &vector, const Vector &other)
{
  return Vector(vector.x * other.x, vector.y * other.y, vector.z * other.z);
}
