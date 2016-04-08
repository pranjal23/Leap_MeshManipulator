#ifndef LEAPACTION_H
#define LEAPACTION_H

#include "commons.h"

class LeapAction
{
public:
    explicit LeapAction();
    ~LeapAction();

    //Set & Get Name
    void setActionName(std::string actName);
    std::string getActionName();

    //Set and Get Rotation Parameters
    void setAngleOfRotation(float angle);
    float getAngleOfRotation();
    void setAxisOfRotation(Vector3D axis);
    Vector3D getAxisOfRotation();

    //Set and Get Pan Motion Parameters
    void setMagnitudeOfMotion(float magnitude);
    float getMagnitudeOfMotion();
    void setAxisOfMotion(Vector3D axis);
    Vector3D getAxisOfMotion();

private:
    //Action name to describe the type of action
    std::string actionName;

    //Angle of rotation and axis of rotation (Quaternion...)
    float angleOfRotation;
    Vector3D axisOfRotation;

    //Magnitude of Camera motion and direction of Camera Motion
    float magnitudeOfMotion;
    Vector3D axisOfMotion;
};

#endif // LEAPACTION_H
