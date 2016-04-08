#include "leapaction.h"

LeapAction::LeapAction()
{

}

LeapAction::~LeapAction()
{

}

//Set & Get Name
void LeapAction::setActionName(std::string actName)
{
    actionName = actName;
}

std::string LeapAction::getActionName()
{
    return actionName;
}

//Set and Get Rotation Parameters
void LeapAction::setAngleOfRotation(float angle)
{
    angleOfRotation = angle;
}

float LeapAction::getAngleOfRotation()
{
    return angleOfRotation;
}

void LeapAction::setAxisOfRotation(Vector3D axis)
{
    axisOfRotation = axis;
}

Vector3D LeapAction::getAxisOfRotation()
{
    return axisOfRotation;
}

//Set and Get Pan Motion Parameters
void LeapAction::setMagnitudeOfMotion(float magnitude)
{
    magnitudeOfMotion = magnitude;
}

float LeapAction::getMagnitudeOfMotion()
{
    return magnitudeOfMotion;
}

void LeapAction::setAxisOfMotion(Vector3D axis)
{
    axisOfMotion = axis;
}

Vector3D LeapAction::getAxisOfMotion()
{
    return axisOfMotion;
}
