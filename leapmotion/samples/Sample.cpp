/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <iostream>
#include <cstring>
#include "Leap.h"

using namespace Leap;
uint8_t prevFingerCount = 0;
int prevNormal = 0;
float prevPinch = 0.0;

class SampleListener : public Listener {
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

const std::string fingerNames[] = {
   "Thumb",
   "Index",
   "Middle",
   "Ring",
   "Pinky"
};
const std::string boneNames[] = {
   "Metacarpal",
   "Proximal",
   "Middle",
   "Distal"
};
const std::string stateNames[] = {
   "STATE_INVALID",
   "STATE_START",
   "STATE_UPDATE",
   "STATE_END"
};

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller & controller) {
   // Get the most recent frame and report some basic information
   const Frame frame = controller.frame();
   uint16_t numberOfFingers = 0;
   HandList hands = frame.hands();
   if (hands.count() == 1) {
      float pinch = hands[0].pinchStrength();
      // documentation sayas pinchStrengh deprecated and to use pinchDistance, button
      // pinchDistance not available as a method.

      if (pinch < 0.89) {
         const Vector normal = hands[0].palmNormal();
         float normalVec = normal.y;
         if (normalVec < 0.0 && prevNormal != -1) {
            std::cout << "2:-1" << std::endl;
            prevNormal = -1;
         } else if (normalVec > 0.0 && prevNormal != 1) {
            std::cout << "2:1" << std::endl;
            prevNormal = 1;
         }
      } else if (prevNormal != 0){
         std::cout << "2:0" << std::endl;
         prevNormal = 0;
      }
   } else if (hands.count() >= 2) {
      // Volume control
      for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
         // Get the first hand
         const Hand hand = * hl;
         const FingerList fingers = hand.fingers();
         numberOfFingers += fingers.extended().count();
      }
      if (prevFingerCount != numberOfFingers) {
         std::cout << "1:" << numberOfFingers * 3 + 120 << std::endl;
         prevFingerCount = numberOfFingers;
      }
   }
}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void SampleListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char** argv) {
  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}
