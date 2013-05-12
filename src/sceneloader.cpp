/*!
 *\file sceneloader.cpp
 *\brief Contains SceneLoader class definition
 */

#include <iostream>
#include <QFile>

#include "sceneloader.h"
#include "objfilereader.h"

/*
* public:
*/
ScenePointer SceneLoader::loadScene(const QString &filePath) const {
  QFile sceneFile(filePath);

  sceneFile.open(QIODevice::ReadOnly);
  if (!sceneFile.isOpen()) {
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
  QString lightSourceType;

  if (!readAttributeAsString(element, "type", lightSourceType)) {
    return LightSourcePointer(NULL);
  }

  Color ambientIntensity;
  Color diffuseIntensity;
  Color specularIntensity;

  if (!readChildElementAsVector(element, "ambient_emission", ambientIntensity) ||
      !readChildElementAsVector(element, "diffuse_emission", diffuseIntensity) ||
      !readChildElementAsVector(element, "specular_emission", specularIntensity)) {
    return LightSourcePointer(NULL);
  }

  if (lightSourceType == "directional") {
    return readDirectedLight(element, ambientIntensity, diffuseIntensity, specularIntensity);
  }
  if (lightSourceType == "point") {
    return readPointLight(element, ambientIntensity, diffuseIntensity, specularIntensity);
  }
  if (lightSourceType == "spotlight") {
    return readSpotLight(element, ambientIntensity, diffuseIntensity, specularIntensity);
  }

  std::cerr << "Scene parsing error: unknown light source type '" << lightSourceType.toUtf8().constData() << "'" << std::endl;
  return LightSourcePointer(NULL);
}

DirectedLightPointer SceneLoader::readDirectedLight(const QDomElement &element, const Color &ambientIntensity, const Color &diffuseIntensity, const Color &specularIntensity) const {
  Vector direction;

  if (readChildElementAsVector(element, "dir", direction)) {
    return DirectedLightPointer(new DirectedLight(ambientIntensity, diffuseIntensity, specularIntensity, direction));
  }

  return DirectedLightPointer(NULL);
}

PointLightPointer SceneLoader::readPointLight(const QDomElement &element, const Color &ambientIntensity, const Color &diffuseIntensity, const Color &specularIntensity) const {
  Vector position;
  float constantAttenutaionCoefficient;
  float linearAttenutaionCoefficient;
  float quadraticAttenutaionCoefficient;

  if (readChildElementAsVector(element, "pos", position) && 
      readChildElementAsFloat(element, "attenuation", "const", constantAttenutaionCoefficient) &&
      readChildElementAsFloat(element, "attenuation", "linear", linearAttenutaionCoefficient) &&
      readChildElementAsFloat(element, "attenuation", "quad", quadraticAttenutaionCoefficient)) {
    return PointLightPointer(new PointLight(ambientIntensity, diffuseIntensity, specularIntensity, position, 
                                            constantAttenutaionCoefficient, linearAttenutaionCoefficient, quadraticAttenutaionCoefficient));
  }
  
  return PointLightPointer(NULL);
}

SpotLightPointer SceneLoader::readSpotLight(const QDomElement &element, const Color &ambientIntensity, const Color &diffuseIntensity, const Color &specularIntensity) const {
  Vector position;
  Vector direction;
  float constantAttenutaionCoefficient;
  float linearAttenutaionCoefficient;
  float quadraticAttenutaionCoefficient;
  float	umbraAngle;
  float	penumbraAngle;
  float	falloffFactor;
  
  if (readChildElementAsVector(element, "pos", position) && 
      readChildElementAsVector(element, "dir", direction) &&
      readChildElementAsFloat(element, "attenuation", "const", constantAttenutaionCoefficient) &&
      readChildElementAsFloat(element, "attenuation", "linear", linearAttenutaionCoefficient) &&
      readChildElementAsFloat(element, "attenuation", "quad", quadraticAttenutaionCoefficient) &&
      readChildElementAsFloat(element, "umbra", "angle", umbraAngle) &&
      readChildElementAsFloat(element, "penumbra", "angle", penumbraAngle) &&
      readChildElementAsFloat(element, "falloff", "value", falloffFactor)) {
    return SpotLightPointer(new SpotLight(ambientIntensity, diffuseIntensity, specularIntensity, position, direction, 
                                          constantAttenutaionCoefficient, linearAttenutaionCoefficient, quadraticAttenutaionCoefficient, 
                                          umbraAngle, penumbraAngle, falloffFactor));
  }

  return SpotLightPointer(NULL);
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
  if (shapeType == "triangle") {
    return readTriangle(element, shapeMaterial);
  }
  if (shapeType == "model") {
    return readMeshModel(element, shapeMaterial);
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

TrianglePointer SceneLoader::readTriangle(const QDomElement &element, MaterialPointer material) const {
  Vector vertex0;
  Vector vertex1;
  Vector vertex2;

  if (readChildElementAsVector(element, "v0", vertex0) &&
      readChildElementAsVector(element, "v1", vertex1) &&
      readChildElementAsVector(element, "v2", vertex2)) {
    return TrianglePointer(new Triangle(vertex0, vertex1, vertex2, material));
  }

  return TrianglePointer(NULL);
}

MeshModelPointer SceneLoader::readMeshModel(const QDomElement &element, MaterialPointer material) const {
  Vector translation;
  Vector scale;
  QString modelFileName;

  if (readChildElementAsVector(element, "translation", translation) &&
      readChildElementAsVector(element, "scale", scale) &&
      readChildElementAsString(element, "model", "file_name", modelFileName)) {
    ObjFileReader objFileReader;
    return objFileReader.readMeshFromObjFile(modelFileName, translation, scale, material);
  }
  
  return MeshModelPointer(NULL);
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


bool SceneLoader::readChildElementAsString(const QDomElement &element, const QString &childElementName, const QString &attributeName, QString &value) const {
  QDomElement childElement = element.firstChildElement(childElementName);

  if (childElement.isNull()) {
    std::cerr << "Scene parsing error: element '" << element.tagName().toUtf8().constData() << "' has no '" << childElementName.toUtf8().constData() << "' child element" << std::endl;
    return false;
  }

  return readAttributeAsString(childElement, attributeName, value);
}