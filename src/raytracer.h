/*!
 *\file raytracer.h
 *\brief Contains RayTracer declaration
 */
#pragma once

#include <QString>
	
#include "scene.h"

class RayTracer {
	public:
		RayTracer() {}
		virtual ~RayTracer() {}

		void renderScene(const Scene &scene);
		void saveRenderedImageToFile(const QString &filePath);
};

