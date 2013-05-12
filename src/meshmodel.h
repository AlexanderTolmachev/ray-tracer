#pragma once

#include <vector>

#include "triangle.h"

class ModelTriangle;

typedef QSharedPointer<ModelTriangle> ModelTrianglePointer;

class ModelTriangle : public Triangle {
  public:
    ModelTriangle(Vector vertex0, Vector vertex1, Vector vertex2,
                  Vector normal0, Vector normal1, Vector normal2,
                  MaterialPointer material);
    virtual ~ModelTriangle();

    virtual Vector getNormal(const Ray &ray, float distance, const RayIntersection &intersection = RayIntersection()) const;

  private:
    Vector mNormal0;
    Vector mNormal1;
    Vector mNormal2;
};

struct BoundingBox {
  
  bool intersectsWithRay(const Ray &ray) const;

  Vector min;
  Vector max;
};

class MeshModel;

typedef QSharedPointer<MeshModel> MeshModelPointer;

class MeshModel : public Shape {
  public:
    MeshModel(const std::vector<ModelTrianglePointer> &triangles, const BoundingBox &boundingBox, MaterialPointer material);
    virtual ~MeshModel();

    virtual RayIntersection intersectWithRay(const Ray &ray) const;
    virtual Vector getNormal(const Ray &ray, float distance, const RayIntersection &intersection = RayIntersection()) const;

  private:
    std::vector<ModelTrianglePointer> mTriangles;
    BoundingBox mBoundingBox;
};