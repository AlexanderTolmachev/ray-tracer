#pragma once

#include "csgbinaryoperationnode.h"

class CSGIntersectionOperation;

typedef QSharedPointer<CSGIntersectionOperation> CSGIntersectionOperationPointer;

class CSGIntersectionOperation : public CSGBinaryOperationNode {
public:
  CSGIntersectionOperation(CSGNodePointer leftArgument, CSGNodePointer rightArgument);
  virtual ~CSGIntersectionOperation();

  virtual RayIntersection intersectWithRay(const Ray &ray) const;
  virtual Vector getNormal(const Ray &ray, float distance) const;
};