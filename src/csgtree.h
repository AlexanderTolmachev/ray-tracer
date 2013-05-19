#pragma once

#include "shape.h"
#include "csgnode.h"

class CSGTree;

typedef QSharedPointer<CSGTree> CSGTreePointer;

class CSGTree : public Shape {
  public:
    CSGTree(CSGNodePointer treeRoot, MaterialPointer material = MaterialPointer(NULL));
    virtual ~CSGTree();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance) const;

  private:
    CSGNodePointer mRoot;
};