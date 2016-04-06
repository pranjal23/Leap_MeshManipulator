#ifndef SAMPLELISTENER_H
#define SAMPLELISTENER_H

#include <iostream>
#include <cstring>
#include "Leap.h"

using namespace Leap;

class LeapMotionListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);

  private:
};

#endif // SAMPLELISTENER_H
