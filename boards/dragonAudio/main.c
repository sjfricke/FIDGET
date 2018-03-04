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

void socketCallback( int type, const char* value) {
  // To declare variables inside case you need to enable a scope with { }
  printf("type: %d\t value: %s\n", type, value);

  // printf("DEBUG - Data called back of %d : %s\n", type, value);
  switch(type) {

  case 0:
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

void* pollButton(void* notinUse){
  struct pollfd pfd;
  int fd;
  char buf;
  GpioInput(115);
  
  fd = open("/sys/class/gpio/gpio115/value", O_RDONLY);
  if (fd < 0) {
    puts("ERROR: Could not open file:\n");
    return (void*) -1;
  }
  
  pfd.fd = fd;
  pfd.events = POLLPRI;
  
  // consume any prior interrupt
  lseek(fd, 0, SEEK_SET);
  read(fd, &buf, 1);
  
  while(1) {
    poll(&pfd, 1, -1);
    
    lseek(fd, 0, SEEK_SET);
    read(fd, &buf, 1);

    // buf is 0x30 or 0x31
    broadcastInt("15", buf & 0x1);
  }
}

void* pollTouch(void* notinUse){
  struct pollfd pfd;
  int fd;
  char buf;
  GpioInput(36);
  
  fd = open("/sys/class/gpio/gpio36/value", O_RDONLY);
  if (fd < 0) {
    puts("ERROR: Could not open file:\n");
    return (void*) -1;
  }
  
  pfd.fd = fd;
  pfd.events = POLLPRI;
  
  // consume any prior interrupt
  lseek(fd, 0, SEEK_SET);
  read(fd, &buf, 1);
  
  while(1) {
    poll(&pfd, 1, -1);
    
    lseek(fd, 0, SEEK_SET);
    read(fd, &buf, 1);

    // buf is 0x30 or 0x31
    broadcastInt("16", buf & 0x1);
  }
}

void* pollAccel(void* notInUse) {

  float x, y, z;
  AccelSetup(0);

  AccelGetValue(0, &a, &b, &c);

  
  
}

int main ( int argc, char* argv[] ) {

  pthread_t button_thread;
  pthread_t touch_tread;
  int rc;
  
  voiceHardwareSetup();
  voiceDictionarySetup();
  
  g_server = (server_t*)malloc(sizeof(server_t));
  g_server->port = 8000;
  g_server->onSocketMessage = socketCallback;

  startServer();

  rc = pthread_create(&button_thread, NULL, pollButton, NULL);
  if (rc) {
    printf("ERROR: Can't create button thread");
  }

  rc = pthread_create(&touch_thread, NULL, pollTouch, NULL);
  if (rc) {
    printf("ERROR: Can't create touch thread");
  }
  
  // main infinite loop
  while(1) {
    voiceCommand();
  }

}
