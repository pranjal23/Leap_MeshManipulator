#include "handmotionconverter.h"

HandMotionConverter::HandMotionConverter(QObject *parent) : QObject(parent)
{

}

void HandMotionConverter::sendZoomAction(ZOOML_TYPE zoom_type, float strength)
{
    //emit sendHandAction(HAND_ACTION::ZOOM_IN);
}

void HandMotionConverter::sendPanAction(PAN_DIRECTION pan_direction, float strength)
{

}

void HandMotionConverter::sendRotateAction(ROTATE_DIRECTION rotate_direction, float strength)
{

}

