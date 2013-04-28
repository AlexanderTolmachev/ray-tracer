/*!
 *\file sceneloader.cpp
 *\brief Contains SceneLoader class definition
 */

#include <iostream>
#include <QFile>

#include "sceneloader.h"

ScenePointer SceneLoader::loadScene(const QString &filePath) {
  QFile sceneFile(filePath);

  sceneFile.open(QIODevice::ReadOnly);
  if (!sceneFile.isOpen())
  {
    std::cerr << "Unable to open file at path " << filePath.toUtf8().constData() << std::endl;
    return ScenePointer(NULL);
  }

  QDomDocument document;
  QString errorMessge;
  int errorLine, errorColumn;

  if (!document.setContent(&sceneFile, &errorMessge, &errorLine, &errorColumn)) {
    std::cerr << "XML parsing error at line " << errorLine << ", cloumn " << errorColumn << ": " << errorMessge.toUtf8().constData() << std::endl;
    return ScenePointer(NULL);    
  }

  ScenePointer scene = ScenePointer(new Scene());
  QDomElement rootElement = document.documentElement();
  if (!read(rootElement, scene)) {
    std::cerr << "Failed scene loading, check scene format" << std::endl;
    return ScenePointer(NULL);    
  }

  return scene;
}

bool SceneLoader::read(const QDomNode &node, ScenePointer scene) {
  return true;
}