/*!
 *\file raytracer.h
 *\brief Contains RayTracer class declaration
 */
#pragma once

#include <QString>
#include <QImage>
  
#include "scene.h"

class RayTracer {
  public:
    RayTracer();
    virtual ~RayTracer();

    void setScene(ScenePointer scene);
    void setImageResolution(int width, int height);
    void renderScene();
    void saveRenderedImageToFile(const QString &filePath);

  private:
    void render();
    Color traceRay(const Ray &ray, int currentRecursionDepth, bool isRayReflected,
                   float refractionCoefficient, float reflectionIntencity, 
                   RayIntersection &intersection);

  private:
    ScenePointer mScene;
    QImage mRenderedImage;
};

