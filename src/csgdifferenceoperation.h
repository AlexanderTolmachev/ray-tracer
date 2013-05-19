#pragma once

#include "csgbinaryoperationnode.h"

class CSGDifferenceOperation;

typedef QSharedPointer<CSGDifferenceOperation> CSGDifferenceOperationPointer;

class CSGDifferenceOperation : public CSGBinaryOperationNode {
public:
  CSGDifferenceOperation(CSGNodePointer leftArgument, CSGNodePointer rightArgument);
  virtual ~CSGDifferenceOperation();

  virtual RayIntersection intersectWithRay(const Ray &ray) const;
  virtual Vector getNormal(const Ray &ray, float distance) const;
};