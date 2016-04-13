#include "leaplistener.h"

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void LeapMotionListener::onInit(const Controller& controller) {
    std::cout << "Initialized" << std::endl;
}

void LeapMotionListener::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
    controller.enableGesture(Gesture::TYPE_CIRCLE);
    controller.enableGesture(Gesture::TYPE_SWIPE);
}

void LeapMotionListener::onDisconnect(const Controller& controller) {
    // Note: not dispatched when running in a debugger.
    std::cout << "Disconnected" << std::endl;
}

void LeapMotionListener::onExit(const Controller& controller) {
    std::cout << "Exited" << std::endl;
}

void LeapMotionListener::setHandMotionConverter(LeapActionSender* handMotionConv)
{
    leapActionSender = handMotionConv;
}

bool firstModeChange = true;
enum MODE {ZOOM, PAN, ROTATE};
MODE currentMode = MODE::ZOOM;
void LeapMotionListener::onFrame(const Controller& controller) {
    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame();

    /*
    if(false)
    {
        std::cout << "Frame id: " << frame.id()
                  << ", timestamp: " << frame.timestamp()
                  << ", hands: " << frame.hands().count()
                  << ", extended fingers: " << frame.fingers().extended().count()
                  << ", tools: " << frame.tools().count()
                  << ", gestures: " << frame.gestures().count() << std::endl;

    }
    */

    HandList hands = frame.hands();
    const Hand leftmost = hands.leftmost();
    const Hand rightmost = hands.rightmost();

    if(leftmost.isLeft() && rightmost.isRight())
    {
        if(firstModeChange)
        {
            firstModeChange = false;
            std::cout << "Action Processing... Change Mode!!!" << std::endl;
            switch(currentMode)
            {
            case ZOOM:
                currentMode = MODE::PAN;
                 std::cout << "PAN" << std::endl;
                break;
            case PAN:
                currentMode = MODE::ROTATE;
                std::cout << "ROTATE" << std::endl;
                break;
            case ROTATE:
                currentMode = MODE::ZOOM;
                std::cout << "ZOOM" << std::endl;
                break;
            }
        }
        else
        {
            std::cout << "Disabled Change Mode!!!" << std::endl;
        }
    }


    if(currentMode!=MODE::ROTATE)
    {

        if(false)
        {
            for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
                // Get the first hand
                const Hand hand = *hl;
                std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
                std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
                          << ", palm position: " << hand.palmPosition() << std::endl;
                // Get the hand's normal vector and direction
                const Vector normal = hand.palmNormal();
                const Vector direction = hand.direction();

                // Calculate the hand's pitch, roll, and yaw angles
                std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
                          << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
                          << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;

            }
        }
        firstModeChange = true;
    }

    // Get gestures
    const GestureList gestures = frame.gestures();
    for (int g = 0; g < gestures.count(); ++g) {
        Gesture gesture = gestures[g];

        switch (gesture.type()) {
        case Gesture::TYPE_CIRCLE:
        {
            if(currentMode!=MODE::ZOOM)
                return;

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

            std::cout << std::string(2, ' ')
                      << "Circle id: " << gesture.id()
                      << ", state: " << stateNames[gesture.state()]
                      << ", progress: " << circle.progress()
                      << ", radius: " << circle.radius()
                      << ", angle " << angleInDegrees
                      <<  ", " << clockwiseness << std::endl;

            if(angleInDegrees > 0)
            {
                LeapAction* e = new LeapAction;
                e->setActionName(actionNames[0]);

                if(clockwisenessInt==1)
                {
                    Vector3D* vect = new Vector3D;
                    vect->x = 0;
                    vect->y = 0;
                    vect->z = 1;
                    e->setAxisOfMotion(vect);
                }
                else if(clockwisenessInt==2)
                {
                    Vector3D* vect = new Vector3D;
                    vect->x = 0;
                    vect->y = 0;
                    vect->z = -1;
                    e->setAxisOfMotion(vect);
                }

                leapActionSender->actionVector->push_back(*e);
                leapActionSender->emitEmptyAction();
            }
            firstModeChange = true;
            break;
        }
        case Gesture::TYPE_SWIPE:
        {
            if(currentMode!=MODE::PAN)
                return;

            SwipeGesture swipe = gesture;

            std::cout << std::string(2, ' ')
                      << "Swipe id: " << gesture.id()
                      << ", state: " << stateNames[gesture.state()]
                      << ", direction: " << swipe.direction()
                      << ", speed: " << swipe.speed() << std::endl;

            float x = swipe.direction().x;
            float y = swipe.direction().y;
            //float z = swipe.direction().z;

            if(x > 0.1 && (y < 0.1 && y > -0.1))
            {
                //SWIPE LEFT
                std::cout << "Swipe LEFT" << std::endl;

                LeapAction* e = new LeapAction;
                e->setActionName(actionNames[1]);

                Vector3D* vect = new Vector3D;
                vect->x = -1;
                vect->y = 0;
                vect->z = 0;
                e->setAxisOfMotion(vect);

                leapActionSender->actionVector->push_back(*e);
                leapActionSender->emitEmptyAction();
            }
            else if(x < -0.1 && (y < 0.1 && y > -0.1))
            {
                //SWIPE RIGHT
                std::cout << "Swipe right" << std::endl;

                LeapAction* e = new LeapAction;
                e->setActionName(actionNames[1]);

                Vector3D* vect = new Vector3D;
                vect->x = 1;
                vect->y = 0;
                vect->z = 0;
                e->setAxisOfMotion(vect);

                leapActionSender->actionVector->push_back(*e);
                leapActionSender->emitEmptyAction();
            }
            else if(y > 0.1 && (x < 0.1 && x > -0.1))
            {
                //SWIPE UP
                std::cout << "Swipe up" << std::endl;

                LeapAction* e = new LeapAction;
                e->setActionName(actionNames[1]);

                Vector3D* vect = new Vector3D;
                vect->x = 0;
                vect->y = 1;
                vect->z = 0;
                e->setAxisOfMotion(vect);

                leapActionSender->actionVector->push_back(*e);
                leapActionSender->emitEmptyAction();
            }
            else if(y < -0.1 && (x < 0.1 && x > -0.1))
            {
                //SWIPE DOWN
                std::cout << "Swipe down" << std::endl;
                LeapAction* e = new LeapAction;
                e->setActionName(actionNames[1]);

                Vector3D* vect = new Vector3D;
                vect->x = 0;
                vect->y = -1;
                vect->z = 0;
                e->setAxisOfMotion(vect);

                leapActionSender->actionVector->push_back(*e);
                leapActionSender->emitEmptyAction();
            }

            firstModeChange = true;
            break;
        }
        default:
            std::cout << std::string(2, ' ')  << "Unknown gesture type." << std::endl;
            break;
        }
    }

    if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
        std::cout << std::endl;
    }

}

void LeapMotionListener::onFocusGained(const Controller& controller) {
    std::cout << "Focus Gained" << std::endl;
}

void LeapMotionListener::onFocusLost(const Controller& controller) {
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
    std::cout << "Service Connected" << std::endl;
}

void LeapMotionListener::onServiceDisconnect(const Controller& controller) {
    std::cout << "Service Disconnected" << std::endl;
}
