#pragma once

#include "types.h"
#include "meshmodel.h"

struct Vertex {
  Vector position;
  Vector normal;
};

class ObjFileReader {
  public:
    MeshModelPointer readMeshFromObjFile(const QString &fileName, const Vector &translation, const Vector &scale, MaterialPointer material) const;
  private:
    Vector readVector(QString line, const QString& prefix) const;
    QStringList readIndicesDescriptor(QString line, const QString& prefix) const;
    void readIndices(const QString& line, unsigned *position, unsigned *normal, unsigned *textureCoordinates) const;
};