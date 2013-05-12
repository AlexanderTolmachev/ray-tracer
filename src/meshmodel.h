#pragma once

#include <vector>

#include "triangle.h"

struct BoundingBox {
  
  bool intersectsWithRay(const Ray &ray) const;

  Vector min;
  Vector max;
};

class MeshModel;

typedef QSharedPointer<MeshModel> MeshModelPointer;

class MeshModel : public Shape {
  public:
    MeshModel(const std::vector<TrianglePointer> &triangles, const BoundingBox &boundingBox, MaterialPointer material);
    virtual ~MeshModel();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance) const;

  private:
    std::vector<TrianglePointer> mTriangles;
    BoundingBox mBoundingBox;
};