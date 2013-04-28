/*!
 *\file sceneloader.h
 *\brief Contains SceneLoader class declaration
 */

#pragma once

#include <QDomDocument>

#include "scene.h"

class SceneLoader {
  public:
    SceneLoader() {}
    virtual ~SceneLoader() {}

    ScenePointer loadScene(const QString &filePath);
  private:
    bool read(const QDomNode &node, ScenePointer scene);

};