#ifndef HANDMOTIONCONVERTER_H
#define HANDMOTIONCONVERTER_H

#include <QObject>
#include "commons.h"

class HandMotionConverter : public QObject
{
    Q_OBJECT
public:
    explicit HandMotionConverter(QObject *parent = 0);

signals:
    void sendHandAction(HAND_ACTION hand_action,
                        ZOOML_TYPE zoom_type,
                        ROTATE_DIRECTION rotate_direction,
                        PAN_DIRECTION pan_direction,
                        float strength);

public slots:
    void sendZoomAction(ZOOML_TYPE zoom_type, float strength);
    void sendPanAction(PAN_DIRECTION pan_direction, float strength);
    void sendRotateAction(ROTATE_DIRECTION rotate_direction, float strength);
};

#endif // HANDMOTIONCONVERTER_H
