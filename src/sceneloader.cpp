/*!
 *\file sceneloader.cpp
 *\brief Contains SceneLoader class definition
 */

#include <iostream>
#include <QFile>

#include "sceneloader.h"

ScenePointer SceneLoader::loadScene(const QString &filePath) const {
  QFile sceneFile(filePath);

  sceneFile.open(QIODevice::ReadOnly);
  if (!sceneFile.isOpen())
  {
    std::cerr << "Unable to open file at path '" << filePath.toUtf8().constData() << "'" << std::endl;
    return ScenePointer(NULL);
  }

  QDomDocument document;
  QString errorMessge;
  int errorLine, errorColumn;

  if (!document.setContent(&sceneFile, &errorMessge, &errorLine, &errorColumn)) {
    std::cerr << "XML parsing error at line " << errorLine << ", column " << errorColumn << ": " << errorMessge.toUtf8().constData() << std::endl;
    return ScenePointer(NULL);    
  }

  QDomElement rootElement = document.documentElement();
  ScenePointer scene = readScene(rootElement);
  if (scene == NULL) {
    std::cerr << "Failed scene file parsing, check scene format" << std::endl;
  }

  return scene;
}

ScenePointer SceneLoader::readScene(const QDomNode &rootNode) const {
  if (rootNode.toElement().tagName() != "scene") {
    std::cerr << "Scene parsing error: invalid root tag name, 'scene' expected" << std::endl;
    return ScenePointer(NULL);
  }

  ScenePointer scene = ScenePointer(new Scene());

  bool isCameraIntialized = false;
  bool isBackgroundMaterialInitialized = false;

  QDomElement element = rootNode.firstChildElement();
  while (!element.isNull()) {
    QString elementTagName = element.tagName();        
    
    if (elementTagName == "camera") {
      if (isCameraIntialized) {
        std::cerr << "Scene parsing error: 'camera' tag occurred twice, only one camera is allowed" << std::endl;
        return ScenePointer(NULL);
      }            
      
      CameraPointer camera = readCamera(element);           
      if (camera == NULL) {
        std::cerr << "Scene parsing error: failed camera parameters reading" << std::endl;
        return ScenePointer(NULL);
      }
      
      scene->setCamera(camera);
      isCameraIntialized = true;
    } else if (elementTagName == "light") {


    } else if (elementTagName == "object") {


    } else if (elementTagName == "background") {
      if (isBackgroundMaterialInitialized) {
        std::cerr << "Scene parsing error: 'background' tag occurred twice" << std::endl;
        return ScenePointer(NULL);
      }

      MaterialPointer material = readMaterial(element);
      if (material == NULL) {
        std::cerr << "Scene parsing error: failed background material parameters reading" << std::endl;
        return ScenePointer(NULL);
      }

      scene->setBackgroundMaterial(material);
      isBackgroundMaterialInitialized = true;
    } else {
      std::cerr << "Scene parsing error: unknown tag '" << elementTagName.toUtf8().constData() << "'" << std::endl;
      return ScenePointer(NULL);
    }

    element = element.nextSiblingElement();
  }
  
  if (!isCameraIntialized) {
    std::cerr << "Scene parsing error: camera parameters are not specified" << std::endl;
    return ScenePointer(NULL);
  }
  if (!isBackgroundMaterialInitialized) {
    std::cerr << "Scene parsing error: background material parameters are not specified" << std::endl;
    return ScenePointer(NULL);
  }

  return scene;
}

CameraPointer SceneLoader::readCamera(const QDomElement &element) const {
  Vector position;
  Vector up;
  Vector lookAt;
  float fov = 0.0f;
  float nearPlaneDistance = 0.0f;

  if (readChildElementAsVector(element, "pos", position) && 
      readChildElementAsVector(element, "up", up) &&
      readChildElementAsVector(element, "look_at", lookAt) && 
      readChildElementAsFloat(element, "fov", "angle", fov) &&
      readChildElementAsFloat(element, "dist_to_near_plane", "dist", nearPlaneDistance)) {
    return CameraPointer(new Camera(position, up, lookAt, fov, nearPlaneDistance));
  }

  return CameraPointer(NULL);
}

MaterialPointer SceneLoader::readMaterial(const QDomElement &element) const {
  QDomElement materialElement = element.firstChildElement("material");
  
  if (materialElement.isNull()) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no 'material' child element" << std::endl;
    return MaterialPointer(NULL);
  }
  
  MaterialPointer material = MaterialPointer(new Material());
  if (readChildElementAsVector(materialElement, "ambient", material->ambientColor) &&
      readChildElementAsVector(materialElement, "diffuse", material->diffuseColor) &&
      readChildElementAsVector(materialElement, "specular", material->specularColor) &&
      readChildElementAsFloat(materialElement, "specular_power", "power", material->specularPower) &&
      readChildElementAsFloat(materialElement, "refraction_coeff", "theta", material->densityFactor) &&
      readChildElementAsFloat(materialElement, "illumination_factors", "illumination_factor", material->illuminationFactor) &&
      readChildElementAsFloat(materialElement, "illumination_factors", "reflection_factor", material->reflectionFactor) &&
      readChildElementAsFloat(materialElement, "illumination_factors", "refraction_factor", material->refractionFactor)) {
        return material;
  }

  return MaterialPointer(NULL);
}

bool SceneLoader::readVector(const QDomElement &element, Vector &vector) const {  
  if (readAttributeAsFloat(element, "x", vector.x) && 
      readAttributeAsFloat(element, "y", vector.y) && 
      readAttributeAsFloat(element, "z", vector.z)) {
    return true;
  }

  return false;
}

bool SceneLoader::readAttributeAsFloat(const QDomElement &element, const QString &attributeName, float &value) const {
  if (element.hasAttribute(attributeName)) {
    bool isConversionOk;
    value = element.attribute(attributeName).toFloat(&isConversionOk);    
    if (isConversionOk) {
      return true;
    } else {
      std::cerr << "Scene parsing error: unable to convert '" << attributeName.toUtf8().constData() << "' attribute of element '" << element.tagName().toUtf8().constData() << "' to float" << std::endl;
      return false;
    }
  }

  std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no attribute with name '" << attributeName.toUtf8().constData() << "'" << std::endl;
  return false;
}

bool SceneLoader::readChildElementAsVector(const QDomElement &element, const QString &childElementName, Vector &vector) const {
  QDomElement childElement = element.firstChildElement(childElementName);

  if (childElement.isNull()) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no '" << childElementName.toUtf8().constData() << "' child element" << std::endl;
    return false;
  }

  return readVector(childElement, vector);
}

bool SceneLoader::readChildElementAsFloat(const QDomElement &element, const QString &childElementName, const QString &attributeName, float &value) const {
  QDomElement childElement = element.firstChildElement(childElementName);

  if (childElement.isNull()) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no '" << childElementName.toUtf8().constData() << "' child element" << std::endl;
    return false;
  }

  return readAttributeAsFloat(childElement, attributeName, value);
}