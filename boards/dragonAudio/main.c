#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h> // open()
#include <pthread.h>

#include "voice.h"
#include "server/server.h"
#include "modules/gpio.h"
#include "modules/accel.h"

extern server_t* g_server;

int S_BUTTON;
int S_TOUCH;
int S_LED;

void socketCallback( int type, const char* value) {
  // To declare variables inside case you need to enable a scope with { }
  printf("type: %d\t value: %s\n", type, value);

  // printf("DEBUG - Data called back of %d : %s\n", type, value);
  switch(type) {

  case 0:
    GpioSetValue(S_LED, atoi(value));
    break;

  case 1: {
    break;
  }
  case 2:
     break;

  default:
    printf("Not a valid type! [%d]\n", type);
    break;
  }
}

void* pollAccel(void* notInUse) {

  char message[64];
  float x, y, z;
  int l_button = 0;
  int l_touch = 0;
  AccelSetup(0);

  while(1) {

    if (l_button == 0 && GpioGetValue(S_BUTTON) == 1) {
      broadcastInt("15", 1);
    }
    l_button = GpioGetValue(S_BUTTON);
    if (l_touch == 0 && GpioGetValue(S_TOUCH) == 1) {
      broadcastInt("16", 1);
    }
    l_touch = GpioGetValue(S_TOUCH);
       
    AccelGetValue(0, &x, &y, &z);
    
    sprintf(message, "%f,%f,%f", x,y,z);
    broadcastString("17", message);
    sleep(1);
  }
}

int main ( int argc, char* argv[] ) {

  pthread_t accel_thread;
  int rc;
  
  //  voiceHardwareSetup();
  voiceDictionarySetup();

  S_TOUCH = GpioInputPin(23);
  S_BUTTON = GpioInputPin(27);
  S_LED = GpioInputPin(33);
  
  g_server = (server_t*)malloc(sizeof(server_t));
  g_server->port = 8000;
  g_server->onSocketMessage = socketCallback;

  startServer();
  
  rc = pthread_create(&accel_thread, NULL, pollAccel, NULL);
  if (rc) {
    printf("ERROR: Can't create accel thread");
  }

  // main infinite loop
  while(1) {
    voiceCommand();
  }

}
