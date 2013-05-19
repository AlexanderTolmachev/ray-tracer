#pragma once 

#include "shape.h"

class CSGNode;

typedef QSharedPointer<CSGNode> CSGNodePointer;

class CSGNode : public Shape {
  public:
    CSGNode(MaterialPointer material = MaterialPointer(NULL)) : Shape(material) {}
    virtual ~CSGNode() {}

    virtual RayIntersection intersectWithRay(const Ray &ray) const = 0;
    virtual Vector getNormal(const Ray &ray, float distance) const = 0;
};
