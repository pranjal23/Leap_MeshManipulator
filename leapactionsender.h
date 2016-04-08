#ifndef HANDMOTIONCONVERTER_H
#define HANDMOTIONCONVERTER_H

#include <QObject>
#include <vector>

#include "commons.h"
#include "leapaction.h"

class LeapActionSender : public QObject
{
    Q_OBJECT
public:
    explicit LeapActionSender(QObject *parent = 0);
    std::vector<LeapAction>* actionVector;

signals:
    void sendLeapAction();

public slots:
    void emitEmptyAction();
};

#endif // HANDMOTIONCONVERTER_H
