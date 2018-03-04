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
#include <sstream>
  /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <sys/types.h>
  #include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <errno.h>

#define IP "192.168.43.105"
#define PORT 5000
#define MAX_MESSAGE_BUFFER 1024

using namespace Leap;
uint8_t prevFingerCount = 0;
int prevNormal = 1;
float prevPinch = 0.0;

struct sockaddr_in server_addr; // socket struct object
int socket_fd;                  // holds socket file discriptor
char msg_buffer_out[MAX_MESSAGE_BUFFER];     // outgoing messages

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

      if (pinch >= 0.89 && prevNormal != 1) {
         std::cout << "2:1 " << pinch << " " << prevNormal << std::endl;
         prevNormal = 1;

         int status = sendto(socket_fd, "2:1" ,4 , 0, (struct sockaddr*) &server_addr, sizeof(server_addr));
         if (status < 0) {
           printf("sendto() ERROR\n"); return;
         }
      } else if ( pinch < 0.89 && prevNormal != 0){
         std::cout << "2:0 " << pinch << " " << prevNormal << std::endl;
         prevNormal = 0;

         int status = sendto(socket_fd, "2:0" ,4 , 0, (struct sockaddr*) &server_addr, sizeof(server_addr));
         if (status < 0) {
           printf("sendto() ERROR\n"); return;
         }
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
         std::string count = "1:";
	      count += std::to_string( numberOfFingers * 3 + 120);
    	   std::cout << count << std::endl;
        	 prevFingerCount = numberOfFingers;
          int status = sendto(socket_fd, count.c_str() ,strlen(count.c_str()) , 0, (struct sockaddr*) &server_addr, sizeof(server_addr));
          if (status < 0) {
           printf("sendto() ERROR\n"); return;
          }
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


  int status; // used to check status returns

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) { printf("socket() ERROR\n"); return 1; }

  server_addr.sin_addr.s_addr = inet_addr(IP); // sets IP of server
  server_addr.sin_family = AF_INET; // uses internet address domain
  server_addr.sin_port = htons(PORT); // sets PORT on server

  status = connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
  if (status < 0) { printf("connect() ERROR\n"); return 2; }


  printf("end connect, %d\n", status);

  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}
