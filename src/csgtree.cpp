
#include "csgtree.h"
#include "rayintersection.h"

CSGTree::CSGTree(CSGNodePointer treeRoot, MaterialPointer material) 
  : Shape(material), 
    mRoot(treeRoot) {
}

CSGTree::~CSGTree() {
}

RayIntersection CSGTree::intersectWithRay(const Ray &ray) const {
  return mRoot->intersectWithRay(ray);
}

Vector CSGTree::getNormal(const Ray &ray, float distance) const {
  // This method is actually never called
  return Vector();
}
