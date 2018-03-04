#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h> // open()
#include <pthread.h>

#include "voice.h"
#include "server/server.h"
#include "modules/gpio.h"

extern server_t* g_server;

#define A1 33
#define A2 34
#define B1 28
#define B2 35

void setStep(int w1, int w2, int x1, int x2) {
  GpioSetValue(A1, w1);
  GpioSetValue(A2, w2);
  GpioSetValue(B1, x1);
  GpioSetValue(B2, x2);
}

void forward(int delay, int steps) {
  int i;
  for (i = 0; i < steps; i++)  {
    setStep(1,0,0,1);
    usleep(delay);
    setStep(0,1,0,1);
    usleep(delay);
    setStep(0,1,1,0);
    usleep(delay);
    setStep(1,0,1,0);
    usleep(delay);
  }
}

void backward(int delay, int steps) {
  int i;
  for (i = 0; i < steps; i++)  {
    setStep(1,0,0,1);
    usleep(delay);
    setStep(0,1,0,1);
    usleep(delay);
    setStep(0,1,1,0);
    usleep(delay);
    setStep(1,0,1,0);
    usleep(delay);
  }
}


void socketCallback( int type, const char* value) {
  // To declare variables inside case you need to enable a scope with { }
  char command_d[256];
  printf("type: %d\t value: %s\n", type, value);
  
  printf("************************\nDEBUG - Data called back of %d : %s\n", type, value);
  switch(type) {

  case 0:
    GpioSetValue(12, atoi(value));
    break;

  case 1: {
    forward(5000, 32);
    break;
  }
  case 2:
    backward(5000, 32);
    break;

  case 3: {
    sprintf(command_d, "amixer -c 0 cset iface=MIXER,name='RX3 Digital Volume' %d", atoi(value));
    system(command_d);
    break;
  }
  case 4:
     break;

  default:
    printf("Not a valid type! [%d]\n", type);
    break;
  }
}

void* pollPhoto(void* notInUse) {

  char command[256];
  sprintf(command, "ffmpeg -i /dev/video0 -s 480x360 -frames 1 ./output.png -y");

  while(1) {
    system(command);
    usleep(500000);
  }

  return NULL;
  
}

int main ( int argc, char* argv[] ) {

  pthread_t photo_thread;
  int rc;
  
  //  voiceHardwareSetup();
  voiceDictionarySetup();

  GpioOutput(12, 1);
  GpioOutput(A1, 0);
  GpioOutput(A2, 0);
  GpioOutput(B1, 0);
  GpioOutput(B2, 0);

  g_server = (server_t*)malloc(sizeof(server_t));
  g_server->port = 8000;
  g_server->onSocketMessage = socketCallback;

  startServer();
  
  rc = pthread_create(&photo_thread, NULL, pollPhoto, NULL);
  if (rc) {
    printf("ERROR: Can't create accel thread");
  }

  // main infinite loop
  while(1) {
    voiceCommand();
  }

}
