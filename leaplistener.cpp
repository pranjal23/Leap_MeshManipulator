#include "leaplistener.h"
#include <QtCore/QtMath>

const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void LeapMotionListener::onInit(const Controller& controller) {
    Q_UNUSED( controller )
    std::cout << "Initialized" << std::endl;
}

void LeapMotionListener::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
    controller.enableGesture(Gesture::TYPE_CIRCLE);
    controller.enableGesture(Gesture::TYPE_SWIPE);
}

void LeapMotionListener::onDisconnect(const Controller& controller) {
    Q_UNUSED( controller )
    // Note: not dispatched when running in a debugger.
    std::cout << "Disconnected" << std::endl;
}

void LeapMotionListener::onExit(const Controller& controller) {
    Q_UNUSED( controller )
    std::cout << "Exited" << std::endl;
}

void LeapMotionListener::setHandMotionConverter(LeapActionSender* handMotionConv)
{
    leapActionSender = handMotionConv;
}

bool firstModeChange = true;
bool debug=false;
int64_t last_ts = 0;
void LeapMotionListener::onFrame(const Controller& controller) {
    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame();

    if(debug)
    {
        std::cout << "Frame id: " << frame.id()
                  << ", timestamp: " << frame.timestamp()
                  << ", hands: " << frame.hands().count()
                  << ", extended fingers: " << frame.fingers().extended().count()
                  << ", tools: " << frame.tools().count()
                  << ", gestures: " << frame.gestures().count() << std::endl;

    }

    HandList hands = frame.hands();
    const Hand leftmost = hands.leftmost();
    const Hand rightmost = hands.rightmost();

    if(hands.count()==2 && leftmost.isLeft() && rightmost.isRight())
    {
        if(firstModeChange)
        {
            firstModeChange = false;
            std::cout << "Action Processing... Change Mode!!!" << std::endl;
            LeapAction* e = new LeapAction;
            e->setActionName(actionNames[3]);
            leapActionSender->actionVector->push_back(*e);
            leapActionSender->emitEmptyAction();
        }
        else
        {
            std::cout << "Disabled Change Mode!!!" << std::endl;
        }
    } else if(hands.count()==1){
        firstModeChange = true;
    }

    if(hands.count()==1)
    {
        // Get the first hand
        const Hand hand = hands.frontmost();
        int fingerCount = hand.fingers().extended().count();

        int64_t curr_ts = frame.timestamp();
        if( (fingerCount < 3) &&(curr_ts - last_ts)>180000)
        {
            // Get the hand's normal vector and direction
            const Vector normal = hand.palmNormal();
            const Vector direction = hand.direction();

            qreal yaw = direction.yaw();
            qreal pitch = direction.pitch();
            qreal roll = normal.roll();
            if(debug)
            {
                // Calculate the hand's pitch, roll, and yaw angles
                std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
                          << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
                          << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;
            }


            if(fingerCount==1)
            {
                LeapAction* e = new LeapAction;
                e->setActionName(actionNames[2]);

                Vector3D* vect = new Vector3D;
                vect->x = pitch;
                vect->y = 0;
                vect->z = 0;
                e->setAxisOfRotation(vect);

                leapActionSender->actionVector->push_back(*e);
                leapActionSender->emitEmptyAction();
                last_ts = frame.timestamp();
            }
            else if(fingerCount==2)
            {
                LeapAction* e = new LeapAction;
                e->setActionName(actionNames[2]);

                Vector3D* vect = new Vector3D;
                vect->x = 0;
                vect->y = yaw;
                vect->z = 0;
                e->setAxisOfRotation(vect);

                leapActionSender->actionVector->push_back(*e);
                leapActionSender->emitEmptyAction();
                last_ts = frame.timestamp();
            }
            else if(fingerCount==0)
            {
                LeapAction* e = new LeapAction;
                e->setActionName(actionNames[2]);

                Vector3D* vect = new Vector3D;
                vect->x = 0;
                vect->y = 0;
                vect->z = roll;
                e->setAxisOfRotation(vect);

                leapActionSender->actionVector->push_back(*e);
                leapActionSender->emitEmptyAction();
                last_ts = frame.timestamp();
            }

        }

        // Gestures - PAN & ZOOM
        const GestureList gestures = frame.gestures();
        for (int g = 0; g < gestures.count(); ++g) {
            Gesture gesture = gestures[g];

            switch (gesture.type()) {

            //ZOOM
            case Gesture::TYPE_CIRCLE:
            {
                int64_t curr_ts = frame.timestamp();
                if((curr_ts - last_ts)>10000)
                {
                    CircleGesture circle = gesture;
                    std::string clockwiseness;
                    int clockwisenessInt = 0;

                    if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
                        clockwiseness = "clockwise";
                        clockwisenessInt = 1;
                    } else {
                        clockwiseness = "counterclockwise";
                        clockwisenessInt = 2;
                    }

                    // Calculate angle swept since last frame
                    float sweptAngle = 0;
                    if (circle.state() != Gesture::STATE_START) {
                        CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
                        sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
                    }
                    float angleInDegrees = sweptAngle * RAD_TO_DEG;

                    if(debug)
                    {
                        std::cout << std::string(2, ' ')
                                  << "Circle id: " << gesture.id()
                                  << ", state: " << stateNames[gesture.state()]
                                  << ", progress: " << circle.progress()
                                  << ", radius: " << circle.radius()
                                  << ", angle " << angleInDegrees
                                  <<  ", " << clockwiseness << std::endl;
                    }

                    if(angleInDegrees > 0)
                    {
                        LeapAction* e = new LeapAction;
                        e->setActionName(actionNames[0]);

                        if(clockwisenessInt==1)
                        {
                            Vector3D* vect = new Vector3D;
                            vect->x = 1;
                            vect->y = 0;
                            vect->z = 1;
                            e->setAxisOfMotion(vect);
                        }
                        else if(clockwisenessInt==2)
                        {
                            Vector3D* vect = new Vector3D;
                            vect->x = -1;
                            vect->y = 0;
                            vect->z = -1;
                            e->setAxisOfMotion(vect);
                        }

                        leapActionSender->actionVector->push_back(*e);
                        leapActionSender->emitEmptyAction();
                    }
                    last_ts = frame.timestamp();
                }

                break;
            }

                //PAN
            case Gesture::TYPE_SWIPE:
            {
                SwipeGesture swipe = gesture;

                if(debug)
                {
                    std::cout << std::string(2, ' ')
                              << "Swipe id: " << gesture.id()
                              << ", state: " << stateNames[gesture.state()]
                              << ", direction: " << swipe.direction()
                              << ", speed: " << swipe.speed() << std::endl;
                }

                float x = swipe.direction().x;
                float y = swipe.direction().y;
                //float z = swipe.direction().z;

                std::cout <<  "Swipe direction X:    " << x
                           <<  ",     Swipe direction Y:    " << y << std::endl;

                float strength = swipe.speed();


                if(y > 0.2 && (x < 0.2 && x > -0.2))
                {
                    //SWIPE UP
                    std::cout << "Swipe UP" << std::endl;

                    LeapAction* e = new LeapAction;
                    e->setActionName(actionNames[1]);

                    Vector3D* vect = new Vector3D;
                    vect->x = 0;
                    vect->y = 1;
                    vect->z = 0;
                    e->setAxisOfMotion(vect);
                    e->setMagnitudeOfMotion(strength);

                    leapActionSender->actionVector->push_back(*e);
                    leapActionSender->emitEmptyAction();
                }

                if(y < -0.2 && (x < 0.2 && x > -0.2))
                {
                    //SWIPE DOWN
                    std::cout << "Swipe DOWN" << std::endl;
                    LeapAction* e = new LeapAction;
                    e->setActionName(actionNames[1]);

                    Vector3D* vect = new Vector3D;
                    vect->x = 0;
                    vect->y = -1;
                    vect->z = 0;
                    e->setAxisOfMotion(vect);
                    e->setMagnitudeOfMotion(strength);

                    leapActionSender->actionVector->push_back(*e);
                    leapActionSender->emitEmptyAction();
                }
                break;
            }
            default:
                std::cout << std::string(2, ' ')  << "Unknown gesture type." << std::endl;
                break;
            }
        }

    }
}

void LeapMotionListener::onFocusGained(const Controller& controller) {
    Q_UNUSED( controller )
    std::cout << "Focus Gained" << std::endl;
}

void LeapMotionListener::onFocusLost(const Controller& controller) {
    Q_UNUSED( controller )
    std::cout << "Focus Lost" << std::endl;
}

void LeapMotionListener::onDeviceChange(const Controller& controller) {
    std::cout << "Device Changed" << std::endl;
    const DeviceList devices = controller.devices();

    for (int i = 0; i < devices.count(); ++i) {
        std::cout << "id: " << devices[i].toString() << std::endl;
        std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
    }
}

void LeapMotionListener::onServiceConnect(const Controller& controller) {
    Q_UNUSED( controller )
    std::cout << "Service Connected" << std::endl;
}

void LeapMotionListener::onServiceDisconnect(const Controller& controller) {
    Q_UNUSED( controller )
    std::cout << "Service Disconnected" << std::endl;
}
