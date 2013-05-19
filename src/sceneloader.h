/*!
 *\file sceneloader.h
 *\brief Contains SceneLoader class declaration
 */

#pragma once

#include <QDomDocument>

#include "scene.h"
#include "directedlight.h"
#include "pointlight.h"
#include "spotlight.h"
#include "plane.h"
#include "sphere.h"
#include "cylinder.h"
#include "cone.h"
#include "triangle.h"
#include "box.h"
#include "meshmodel.h"
#include "csgtree.h"
#include "csgbinaryoperationnode.h"
#include "csgshapenode.h"

class SceneLoader {
  public:
    SceneLoader() {}
    virtual ~SceneLoader() {}

    ScenePointer loadScene(const QString &filePath) const;

  private:
    ScenePointer readScene(const QDomNode &rootNode) const;

    CameraPointer readCamera(const QDomElement &element) const;
    LightSourcePointer readLightSource(const QDomElement &element) const;
    ShapePointer readShape(const QDomElement &element) const;
    CSGTreePointer readCSGTree(const QDomElement &element) const;
    MaterialPointer readMaterial(const QDomElement &element) const;

    DirectedLightPointer readDirectedLight(const QDomElement &element, const Color &ambientIntensity, const Color &diffuseIntensity, const Color &specularIntensity) const;
    PointLightPointer readPointLight(const QDomElement &element, const Color &ambientIntensity, const Color &diffuseIntensity, const Color &specularIntensity) const;
    SpotLightPointer readSpotLight(const QDomElement &element, const Color &ambientIntensity, const Color &diffuseIntensity, const Color &specularIntensity) const;

    PlanePointer readPlane(const QDomElement &element, MaterialPointer material) const;
    ShapePointer readSphere(const QDomElement &element, MaterialPointer material) const;
    CylinderPointer readCylinder(const QDomElement &element, MaterialPointer material) const;
    ConePointer readCone(const QDomElement &element, MaterialPointer material) const;
    TrianglePointer readTriangle(const QDomElement &element, MaterialPointer material) const;
    BoxPointer readBox(const QDomElement &element, MaterialPointer material) const;
    MeshModelPointer readMeshModel(const QDomElement &element, MaterialPointer material) const;

    CSGNodePointer readCSGNode(const QDomElement &element) const;
    CSGBinaryOperationNodePointer readCSGOperationNode(const QDomElement &element) const;
    CSGShapeNodePointer readCSGShapeNode(const QDomElement &element) const;

    bool readVector(const QDomElement &element, Vector &vector) const;
    bool readAttributeAsFloat(const QDomElement &element, const QString &attributeName, float &value) const;
    bool readAttributeAsString(const QDomElement &element, const QString &attributeName, QString &value) const;
    bool readChildElementAsVector(const QDomElement &element, const QString &childElementName, Vector &vector) const;
    bool readChildElementAsFloat(const QDomElement &element, const QString &childElementName, const QString &attributeName, float &value) const;
    bool readChildElementAsString(const QDomElement &element, const QString &childElementName, const QString &attributeName, QString &value) const;
};