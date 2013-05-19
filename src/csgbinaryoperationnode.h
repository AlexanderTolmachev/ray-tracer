#pragma once

#include "csgnode.h"

class CSGBinaryOperationNode;

typedef QSharedPointer<CSGBinaryOperationNode> CSGBinaryOperationNodePointer;

class CSGBinaryOperationNode : public CSGNode {
public:
  CSGBinaryOperationNode(CSGNodePointer leftArgument, CSGNodePointer rightArgument)
    : mLeftArgument(leftArgument), 
      mRightArgument(rightArgument) {}
  virtual ~CSGBinaryOperationNode() {}

  virtual RayIntersection intersectWithRay(const Ray &ray) const = 0;
  virtual Vector getNormal(const Ray &ray, float distance) const = 0;

protected:
  CSGNodePointer mLeftArgument;
  CSGNodePointer mRightArgument;
};