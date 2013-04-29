/*!
 *\file main.cpp
 *\brief Application entry point
 */

#include <iostream>
#include <QtCore/QCoreApplication>

#include "inputparameters.h"
#include "sceneloader.h"
#include "raytracer.h"

void printUsage();

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  if (app.arguments().size() == 2 && app.arguments().at(1) == "--help") {
    printUsage();
    return 0;
  }

  InputParametersParser paramatersParser;
  InputParametersPointer inputParameters = paramatersParser.parseInputParameters(app.arguments());  
  if (inputParameters == NULL) {
    std::cout << "Invalid arguments passed to application" << std::endl;
    printUsage();
    return -1;
  }

  std::cout << "Loading scene..." << std::endl; 

  SceneLoader sceneLoader;
  ScenePointer scene = sceneLoader.loadScene(inputParameters->sceneFilePath);
  if (scene == NULL) {
    std::cout << "Scene loading failed" << std::endl;
    return -1;    
  }

  std::cout << "Loading scene finished" << std::endl; 


}

void printUsage() {
  std::cout << "Usage: ray-tracer.exe --scene=scene.xml --resolution_x=1280 --resolution_y=800 --output=image.png" << std::endl;
}