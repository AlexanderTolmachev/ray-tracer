#include "csgshapenode.h"
#include "rayintersection.h"

CSGShapeNode::CSGShapeNode(ShapePointer shape) 
  : CSGNode(shape->getMaterial()),
    mShape(shape) {
}

CSGShapeNode::~CSGShapeNode() {
}

RayIntersection CSGShapeNode::intersectWithRay(const Ray &ray) const {
  return mShape->intersectWithRay(ray);
}

Vector CSGShapeNode::getNormal(const Ray &ray, float distance) const {
  return mShape->getNormal(ray, distance);
}
