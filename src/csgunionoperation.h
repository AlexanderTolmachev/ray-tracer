#pragma once

#include "csgbinaryoperationnode.h"

class CSGUnionOperation;

typedef QSharedPointer<CSGUnionOperation> CSGUnionOperationPointer;

class CSGUnionOperation : public CSGBinaryOperationNode {
public:
  CSGUnionOperation(CSGNodePointer leftArgument, CSGNodePointer rightArgument);
  virtual ~CSGUnionOperation();

  virtual RayIntersection intersectWithRay(const Ray &ray) const;
  virtual Vector getNormal(const Ray &ray, float distance) const;
};