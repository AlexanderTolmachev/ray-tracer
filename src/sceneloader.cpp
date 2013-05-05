/*!
 *\file sceneloader.cpp
 *\brief Contains SceneLoader class definition
 */

#include <iostream>
#include <QFile>

#include "sceneloader.h"

/*
* public:
*/
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

/*
* private:
*/
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
      LightSourcePointer lightSource = readLightSource(element);
      if (lightSource == NULL) {
        std::cerr << "Scene parsing error: failed light source parameters reading" << std::endl;
        return ScenePointer(NULL);
      }
      scene->addLightSource(lightSource);
    } else if (elementTagName == "object") {
      ShapePointer shape = readShape(element);
      if (shape == NULL) {
        std::cerr << "Scene parsing error: failed shape parameters reading" << std::endl;
        return ScenePointer(NULL);
      }
      scene->addShape(shape);
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

LightSourcePointer SceneLoader::readLightSource(const QDomElement &element) const {
  LightSourcePointer lightSource = LightSourcePointer(new LightSource());

  if (!readLightSourceType(element, lightSource->type)) {
    return LightSourcePointer(NULL);
  }

  if (lightSource->type == DIRECTIONAL || lightSource->type == SPOT) {
    if (!readChildElementAsVector(element, "dir", lightSource->direction)) {
      return LightSourcePointer(NULL);
    }
  }  
  
  if (lightSource->type == POINT || lightSource->type == SPOT) {
    if (!readChildElementAsVector(element, "pos", lightSource->position)) {
      return LightSourcePointer(NULL);
    }   
  }

  if (!readChildElementAsVector(element, "ambient_emission", lightSource->ambientIntensity) ||
      !readChildElementAsVector(element, "diffuse_emission", lightSource->diffuseIntensity) ||
      !readChildElementAsVector(element, "specular_emission", lightSource->specularIntensity)) {
    return LightSourcePointer(NULL);
  }

  if (lightSource->type == POINT || lightSource->type == SPOT) {
    if (!readChildElementAsFloat(element, "attenuation", "const", lightSource->constantAttenutaionCoefficient) ||
        !readChildElementAsFloat(element, "attenuation", "linear", lightSource->linearAttenutaionCoefficient) ||
        !readChildElementAsFloat(element, "attenuation", "quad", lightSource->quadraticAttenutaionCoefficient)) {
      return LightSourcePointer(NULL);
    }
  }

  if (lightSource->type == SPOT) {
    if (!readChildElementAsFloat(element, "umbra", "angle", lightSource->umbraAngle) ||
        !readChildElementAsFloat(element, "penumbra", "angle", lightSource->penumbraAngle) ||
        !readChildElementAsFloat(element, "falloff", "value", lightSource->falloffFactor)) {
      return LightSourcePointer(NULL);         
    }
  }

  if (lightSource->type == DIRECTIONAL) {
    if (!readChildElementAsFloat(element, "range", "value", lightSource->range)) {
      return LightSourcePointer(NULL);         
    }
  }

  return lightSource;
}

bool SceneLoader::readLightSourceType(const QDomElement &element, LightSourceType &lightSourceType) const {
  QString sourceType;
  if (!readAttributeAsString(element, "type", sourceType)) {
    return false;
  }

  if (sourceType == "point") {
    lightSourceType = POINT;
    return true;
  }
  if (sourceType == "directional") {
    lightSourceType = DIRECTIONAL;
    return true;
  }
  if (sourceType == "spotlight") {
    lightSourceType = SPOT;
    return true;    
  }

  std::cerr << "Scene parsing error: unknown light source type '" << sourceType.toUtf8().constData() << "'" << std::endl;
  return false;
}

ShapePointer SceneLoader::readShape(const QDomElement &element) const {
  QString shapeType;
  if (!readAttributeAsString(element, "type", shapeType)) {
    return ShapePointer(NULL);
  }

  MaterialPointer shapeMaterial = readMaterial(element);
  if (shapeMaterial == NULL) {
    return ShapePointer(NULL);
  }

  if (shapeType == "plane") {
    return readPlane(element, shapeMaterial);
  }
  if (shapeType == "sphere") {
    return readSphere(element, shapeMaterial);
  }
  if (shapeType == "cylinder") {
    return readCylinder(element, shapeMaterial);
  }
  if (shapeType == "cone") {
    return readCone(element, shapeMaterial);
  }

  // TODO process others
  std::cerr << "Scene parsing error: unknown shape type '" << shapeType.toUtf8().constData() << "'" << std::endl;
  return ShapePointer(NULL);
}

PlanePointer SceneLoader::readPlane(const QDomElement &element, MaterialPointer material) const {
  Vector normal;
  float distance;

  if (readChildElementAsVector(element, "normal", normal) && 
      readChildElementAsFloat(element, "D", "d", distance)) {
    return PlanePointer(new Plane(normal, distance, material));
  }

  return PlanePointer(NULL);
}

ShapePointer SceneLoader::readSphere(const QDomElement &element, MaterialPointer material) const {
  Vector center;
  float radius;

  if (readChildElementAsVector(element, "center", center) &&
      readChildElementAsFloat(element, "radius", "r", radius)) {
    return SpherePointer(new Sphere(center, radius, material));
  }

  return SpherePointer(NULL);
}

CylinderPointer SceneLoader::readCylinder(const QDomElement &element, MaterialPointer material) const {
  Vector topCenter;
  Vector bottomCenter;
  float radius;

  if (readChildElementAsVector(element, "top", topCenter) &&
      readChildElementAsVector(element, "bottom", bottomCenter) &&
      readChildElementAsFloat(element, "radius", "r", radius)) {
    return CylinderPointer(new Cylinder(topCenter, bottomCenter, radius, material));
  }

  return CylinderPointer(NULL);
}

ConePointer SceneLoader::readCone(const QDomElement &element, MaterialPointer material) const {
  Vector top;
  Vector bottomCenter;
  float radius;

  if (readChildElementAsVector(element, "top", top) &&
      readChildElementAsVector(element, "bottom", bottomCenter) &&
      readChildElementAsFloat(element, "radius", "r", radius)) {
    return ConePointer(new Cone(top, bottomCenter, radius, material));
  }

  return ConePointer(NULL);
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
  if (!element.hasAttribute(attributeName)) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no attribute with name '" << attributeName.toUtf8().constData() << "'" << std::endl;
    return false;
  }

  bool isConversionOk;
  value = element.attribute(attributeName).toFloat(&isConversionOk);    
  if (!isConversionOk) {
    std::cerr << "Scene parsing error: unable to convert '" << attributeName.toUtf8().constData() << "' attribute of element '" << element.tagName().toUtf8().constData() << "' to float" << std::endl;
    return false;
  }

  return true;
}

bool SceneLoader::readAttributeAsString(const QDomElement &element, const QString &attributeName, QString &value) const {
  if (!element.hasAttribute(attributeName)) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no attribute with name '" << attributeName.toUtf8().constData() << "'" << std::endl;
    return false;
  }

  value = element.attribute(attributeName);
  return true;
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

