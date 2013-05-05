/*!
 *\file sceneloader.h
 *\brief Contains SceneLoader class declaration
 */

#pragma once

#include <QDomDocument>

#include "scene.h"
#include "plane.h"
#include "sphere.h"
#include "cylinder.h"

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
    MaterialPointer readMaterial(const QDomElement &element) const;

    PlanePointer readPlane(const QDomElement &element, MaterialPointer material) const;
    ShapePointer readSphere(const QDomElement &element, MaterialPointer material) const;
    CylinderPointer readCylinder(const QDomElement &element, MaterialPointer material) const;

    bool readLightSourceType(const QDomElement &element, LightSourceType &lightSourceType) const;
    bool readVector(const QDomElement &element, Vector &vector) const;
    bool readAttributeAsFloat(const QDomElement &element, const QString &attributeName, float &value) const;
    bool readAttributeAsString(const QDomElement &element, const QString &attributeName, QString &value) const;
    bool readChildElementAsVector(const QDomElement &element, const QString &childElementName, Vector &vector) const;
    bool readChildElementAsFloat(const QDomElement &element, const QString &childElementName, const QString &attributeName, float &value) const;
};