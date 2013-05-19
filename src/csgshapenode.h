#pragma once

#include "csgnode.h"

class CSGShapeNode;

typedef QSharedPointer<CSGShapeNode> CSGShapeNodePointer;

class CSGShapeNode : public CSGNode {
public:
  CSGShapeNode(ShapePointer shape);
  virtual ~CSGShapeNode();

  virtual RayIntersection intersectWithRay(const Ray &ray) const;
  virtual Vector getNormal(const Ray &ray, float distance) const;

private:
  ShapePointer mShape;
};