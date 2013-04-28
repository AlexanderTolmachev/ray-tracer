/*!
 *\file inputparameters.h
 *\brief Contains InputParameters and InputParametersParser classes declaration
 */

#pragma once

#include <QSharedPointer>
#include <QString>
#include <QRegExp>
#include <QStringList>

struct InputParameters;

typedef QSharedPointer<InputParameters> InputParametersPointer;

struct InputParameters {
  QString sceneFilePath;
  QString outputFilePath;
  int xResolution, yResolution;
};

class InputParametersParser {
  public:
    InputParametersParser();
    virtual ~InputParametersParser();

    InputParametersPointer parseInputParameters(QStringList args) const;

  private:
    QRegExp mSceneArgumentRegex;
    QRegExp mOutputArgumentRegex;
    QRegExp mXResolutionArgumentRegex;
    QRegExp mYResolutionArgumentRegex;
};