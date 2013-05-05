/*!
 *\file raytracer.cpp
 *\brief Contains RayTracer class definition
 */

#include "raytracer.h"

#define MAX_TRACER_RECURSION_DEPTH 10
#define RGBA(r, g, b, a) ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);

/*
* public:
*/
RayTracer::RayTracer() 
  : mScene(NULL) {
}

RayTracer::~RayTracer() {
}

void RayTracer::setScene(ScenePointer scene) {
  mScene = scene;
}

void RayTracer::setImageResolution(int width, int height) {
  mScene->getCamera()->setImageResolution(width, height);
}

void RayTracer::renderScene() {
  mRenderedImage = QImage(mScene->getCamera()->getImageWidth(), mScene->getCamera()->getImageHeight(), QImage::Format_RGB32);
  render();
}

void RayTracer::saveRenderedImageToFile(const QString& filePath) {
  mRenderedImage.save(filePath);
}

/*
* private:
*/
void RayTracer::render() {
  int imageWidth = mScene->getCamera()->getImageWidth();
  int imageHeight = mScene->getCamera()->getImageHeight();
  CameraPointer camera = mScene->getCamera();
  unsigned *renderedImageData = reinterpret_cast< unsigned* >(mRenderedImage.bits());

  for (int y = 0; y < imageHeight; ++y) {
    for (int x = 0; x < imageWidth; ++x) {
      RayIntersection intersection;

      Ray ray = camera->emitRay(x, y);
      Color pixelColor = traceRay(ray, 
                                  0,      // initial recursion depth is 0
                                  false,  // ray emitted from camera is not reflected
                                  1.0,    // air refraction coefficient
                                  1.0,    // initial reflection
                                  intersection);
      
      unsigned char redComponent   = static_cast<unsigned char>(std::min<unsigned>(pixelColor.r * 255, 255));
      unsigned char greenComponent = static_cast<unsigned char>(std::min<unsigned>(pixelColor.g * 255, 255)); 
      unsigned char blueComponent  = static_cast<unsigned char>(std::min<unsigned>(pixelColor.b * 255, 255));

      int index = y * imageWidth + x;
      *(renderedImageData + index) = RGBA(redComponent, greenComponent, blueComponent, 255);
    }
  }
}

Color RayTracer::traceRay(const Ray &ray, int currentRecursionDepth, bool isRayReflected,
                          float refractionCoefficient, float reflectionIntencity, 
                          RayIntersection &intersection) {
  if (currentRecursionDepth > MAX_TRACER_RECURSION_DEPTH) {
    return Color();
  }

  intersection = mScene->calculateNearestIntersection(ray);
  
  if (!intersection.rayIntersectsWithShape)
  {
    if (isRayReflected)
    {
      return Color();
    }
    return mScene->getBackgroundMaterial()->ambientColor;
  }

  Vector intersectionPoint = ray.getPointAt(intersection.distanceFromRayOrigin);
  ShapePointer shape = intersection.shape;
  Vector normal = intersection.normalAtInresectionPoint;

  return mScene->calculateIlluminationColor(ray, intersection.distanceFromRayOrigin, normal, shape->getMaterial());

  // TODO Process reflection and refraction
}