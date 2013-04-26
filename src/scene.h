/*!
 *\file scene.h
 *\brief Contains Scene declaration
 */

#pragma once

#include <QSharedPointer>

class Scene;

typedef QSharedPointer<Scene> ScenePointer;

class Scene {
	public:
		Scene() {}
		virtual ~Scene() {}

};
