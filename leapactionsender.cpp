#include "leapactionsender.h"

LeapActionSender::LeapActionSender(QObject *parent) : QObject(parent)
{
    actionVector = new std::vector<LeapAction>;
}

void LeapActionSender::emitEmptyAction()
{
    emit sendLeapAction();
}
