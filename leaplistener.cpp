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
    controller.enableGesture(Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
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

    if(leftmost.isRight() && rightmost.isLeft())
    {
        std::cout << "Action Processing... " << std::endl;
    }

    /*
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
    */


    // Get gestures
    const GestureList gestures = frame.gestures();
    for (int g = 0; g < gestures.count(); ++g) {
        Gesture gesture = gestures[g];

        switch (gesture.type()) {
        case Gesture::TYPE_CIRCLE:
        {
            CircleGesture circle = gesture;
            std::string clockwiseness;

            if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
                clockwiseness = "clockwise";
            } else {
                clockwiseness = "counterclockwise";
            }

            // Calculate angle swept since last frame
            float sweptAngle = 0;
            if (circle.state() != Gesture::STATE_START) {
                CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
                sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
            }
            std::cout << std::string(2, ' ')
                      << "Circle id: " << gesture.id()
                      << ", state: " << stateNames[gesture.state()]
                      << ", progress: " << circle.progress()
                      << ", radius: " << circle.radius()
                      << ", angle " << sweptAngle * RAD_TO_DEG
                      <<  ", " << clockwiseness << std::endl;

            LeapAction* e = new LeapAction;
            e->setActionName(actionNames[0]);
            leapActionSender->actionVector->push_back(*e);
            leapActionSender->emitEmptyAction();
            break;
        }
        case Gesture::TYPE_SWIPE:
        {
            SwipeGesture swipe = gesture;
            std::cout << std::string(2, ' ')
                      << "Swipe id: " << gesture.id()
                      << ", state: " << stateNames[gesture.state()]
                      << ", direction: " << swipe.direction()
                      << ", speed: " << swipe.speed() << std::endl;


            break;
        }
        default:
            std::cout << std::string(2, ' ')  << "Unknown gesture type." << std::endl;
            break;
        }
    }


    if(false)
    {
        if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
            std::cout << std::endl;
        }
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
