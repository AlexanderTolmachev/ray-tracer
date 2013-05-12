
#include <vector>
#include <QFile>
#include <QStringList>

#include "objfilereader.h"
#include "mathcommons.h"

MeshModelPointer ObjFileReader::readMeshFromObjFile(const QString &fileName, const Vector &translation, const Vector &scale, MaterialPointer material) const {
  QFile meshFile(fileName);

  meshFile.open(QIODevice::ReadOnly);
  if (!meshFile.isOpen()) {
    std::cerr << "Scene parsing error: Unable to open file at path '" << fileName.toUtf8().constData() << "'" << std::endl;
    return MeshModelPointer(NULL);
  }

  std::vector<Vector> positions;
  std::vector<Vector> normals;

  std::vector<Vertex>	  faceVertices;
  std::vector<unsigned> faceVerticesIndices;
  std::vector<unsigned> faceIndices;

  // Triangle vertices
  std::vector<Vertex>	vertices;
  // Vertex indices for triangles
  std::vector<unsigned> indices;

  // Read lines, loop will be terminated by break
  while (true) {
    QString line = meshFile.readLine();
    if (meshFile.atEnd()) {
      break;
    }

    if (line.contains("#")) {
      // Skip comment line
    } else if (line.startsWith("v ")) {
      // Vertex line
      Vector position = readVector(line, "v");
      // Apply scale and translation
      Vector modelPosition = componentwiseProduct(position, scale) + translation;
      positions.push_back(modelPosition);
    } else if (line.startsWith("vt ")) {
      // Texture coordinates line
      // Skip
    } else if (line.startsWith("vn ")) {
      // Normal vector line
      Vector normal = readVector(line, "vn");
      normals.push_back(normal);
    } else if (line.startsWith("f ")) {
      faceVertices.clear();
      faceVerticesIndices.clear();

      QStringList indicesDescs = readIndicesDescriptor(line, "f");

      foreach (const QString& index, indicesDescs) {
        unsigned position, texcoord, normal;
        Vertex v;

        readIndices(index, &position, &normal, &texcoord);

        // OBJ uses 1-based arrays
        if (!positions.empty()) {
          v.position  = positions[position - 1];
        }
        if (!normals.empty()) {
          v.normal    = normals[normal - 1];
        }
        // Ignore text coords

        faceVertices.push_back(v);
        faceVerticesIndices.push_back(position);
      }
      
      const unsigned count = faceVertices.size();
      faceIndices.resize(count);
      for (unsigned idx = 0; idx < count; ++idx) {
        // Triangle strip
        if (idx > 2) {
          indices.push_back( faceIndices[0] );
          indices.push_back( faceIndices[idx - 1] );
        }

        faceIndices[idx] = vertices.size();
        vertices.push_back(faceVertices[idx]);
        indices.push_back(faceIndices[idx]);
      }
    }
  }

  std::vector<TrianglePointer> triangles;

  // Create triangles
  for (int idx = 0, count = indices.size(); idx < count; idx += 3)
  {
    Vertex a = vertices[indices[idx]];
    Vertex b = vertices[indices[idx + 1]];
    Vertex c = vertices[indices[idx + 2]];

    triangles.push_back(ModelTrianglePointer(new ModelTriangle(a.position,  b.position, c.position, 
                                                               a.normal,	b.normal,	c.normal,
                                                               MaterialPointer(NULL))));  // material is owned by model
  }

  // Construct bounding box
  Vector boundingBoxMin;
  Vector boundingBoxMax;
  for each (auto position in positions) {
    if (position.x < boundingBoxMin.x) {
      boundingBoxMin.x = position.x;
    }
    if (position.y < boundingBoxMin.y) {
      boundingBoxMin.y = position.y;
    }
    if (position.z < boundingBoxMin.z) {
      boundingBoxMin.z = position.z;
    }

    if (position.x > boundingBoxMax.x) {
      boundingBoxMax.x = position.x;
    }
    if (position.y > boundingBoxMax.y) {
      boundingBoxMax.y = position.y;
    }
    if (position.z > boundingBoxMax.z) {
      boundingBoxMax.z = position.z;
    }
  }

  BoundingBox meshBoundingBox;
  meshBoundingBox.min = boundingBoxMin;
  meshBoundingBox.max = boundingBoxMax;
  
  return MeshModelPointer(new MeshModel(triangles, meshBoundingBox, material));
}

Vector ObjFileReader::readVector(QString line, const QString& prefix) const {
  QString	string = line.remove(prefix + " ").remove("\n").remove("\r");
  QStringList coordinates = string.split(" ", QString::SkipEmptyParts);
  coordinates.removeAll(" ");
  coordinates.removeAll("");
  coordinates.removeAll("\n");
  
  return Vector(coordinates[0].toFloat(), coordinates[1].toFloat(), coordinates[2].toFloat());
}

QStringList ObjFileReader::readIndicesDescriptor(QString line, const QString& prefix) const {
  QString			 indicesString = line.remove(prefix + " ");
  QStringList	 descs = indicesString.split(" ");
  return descs;
}

void ObjFileReader::readIndices(const QString& line, unsigned *position, unsigned *normal, unsigned *textureCoordinates) const {
  QStringList indices = line.split("/");
  *position = indices[0].toUInt();
  *textureCoordinates = indices[1].toUInt();
  *normal   = indices[2].toUInt();
}
