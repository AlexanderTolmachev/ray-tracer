/*!
 *\file scene.h
 *\brief Contains Scene declaration
 */

#pragma once

#include <QSharedPointer>
#include <vector>

#include "shape.h"
#include "lightsource.h"
#include "material.h"

class Scene;

typedef QSharedPointer<Scene> ScenePointer;

class Scene {
	public:
		Scene() {}
		virtual ~Scene() {}

	private:
		std::vector<Shape *> mShapes;
		std::vector<LightSource *> mLightSources;
		MaterialPointer mBackgroundMaterial;
		
};
