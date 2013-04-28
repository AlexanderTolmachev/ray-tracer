/*!
 *\file inputparameters.cpp
 *\brief Contains InputParametersParser classes definition
 */

#include <iostream>

#include "inputparameters.h"

InputParametersParser::InputParametersParser() 
  : mSceneArgumentRegex("--scene=(\\S+)"),
    mOutputArgumentRegex("--output=(\\S+)"),
    mXResolutionArgumentRegex("--resolution_x=(\\d+)"),
    mYResolutionArgumentRegex("--resolution_y=(\\d+)") {
}

InputParametersParser::~InputParametersParser() {
}

InputParametersPointer InputParametersParser::parseInputParameters(QStringList args) const {
  InputParametersPointer inputParameters = InputParametersPointer(new InputParameters());
  bool isSceneParameterInitialized = false;
  bool isOutputParameterInitialized = false;
  bool isXResolutionParameterInitialized = false;
  bool isYResolutionParameterInitialized = false;

  for (int i = 1; i < args.size(); ++i) {
    if (mSceneArgumentRegex.indexIn(args.at(i)) != -1 ) {
      if (isSceneParameterInitialized) {
        std::cerr << "Input arguments parse error: 'scene' argument occurred twice" << std::endl;
        return InputParametersPointer(NULL);
      }
      inputParameters->sceneFilePath = mSceneArgumentRegex.cap(1);
      isSceneParameterInitialized = true;
    } else if (mOutputArgumentRegex.indexIn(args.at(i)) != -1 ) {
      if (isOutputParameterInitialized) {
        std::cerr << "Input arguments parse error: 'output' argument occurred twice" << std::endl;
        return InputParametersPointer(NULL);
      }
      inputParameters->outputFilePath = mOutputArgumentRegex.cap(1);
      isOutputParameterInitialized = true;
    } else if (mXResolutionArgumentRegex.indexIn(args.at(i)) != -1 ) {   
      if (isXResolutionParameterInitialized) {
        std::cerr << "Input arguments parse error: 'resolution_x' argument occurred twice" << std::endl;
        return InputParametersPointer(NULL);
      }
      inputParameters->xResolution = mXResolutionArgumentRegex.cap(1).toInt();
      isXResolutionParameterInitialized = true;
    } else if (mYResolutionArgumentRegex.indexIn(args.at(i)) != -1 ) {
      if (isYResolutionParameterInitialized) {
        std::cerr << "Input arguments parse error: 'resolution_y' argument occurred twice" << std::endl;
        return InputParametersPointer(NULL);
      }
      inputParameters->yResolution = mYResolutionArgumentRegex.cap(1).toInt();
      isYResolutionParameterInitialized = true;
    } else {
      std::cerr << "Input arguments parse error: unknown argument " << args.at(i).toUtf8().constData() << std::endl;
      return InputParametersPointer(NULL);
    }
  }

  if (!isSceneParameterInitialized) {
    std::cerr << "Input arguments parse error: 'scene' argument is not specified" << std::endl;
    return InputParametersPointer(NULL);
  } else if (!isOutputParameterInitialized) {
    std::cerr << "Input arguments parse error: 'output' argument is not specified" << std::endl;
    return InputParametersPointer(NULL);
  } else if (!isXResolutionParameterInitialized) {
    std::cerr << "Input arguments parse error: 'resolution_x' argument is not specified" << std::endl;
    return InputParametersPointer(NULL);
  } else if (!isYResolutionParameterInitialized) {
    std::cerr << "Input arguments parse error: 'resolution_y' argument is not specified" << std::endl;
    return InputParametersPointer(NULL);
  }

  return inputParameters;
}