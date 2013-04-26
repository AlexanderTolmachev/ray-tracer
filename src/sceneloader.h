/*!
 *\file sceneloader.h
 *\brief Contains SceneLoader declaration
 */

#pragma once

#include "scene.h"

class SceneLoader {
	public:
		SceneLoader() {}
		virtual ~SceneLoader() {}

		ScenePointer loadScene(const QString &filePath);
};