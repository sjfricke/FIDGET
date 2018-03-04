#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h> // open()
#include <pthread.h>

#include "server/server.h"
#include "modules/gpio.h"

extern server_t* g_server;

void socketCallback( int type, const char* value) {
  // To declare variables inside case you need to enable a scope with { }
  printf("type: %d\t value: %s\n", type, value);
  broadcastInt("2", 4);
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


int main ( int argc, char* argv[] ) {

  g_server = (server_t*)malloc(sizeof(server_t));
  g_server->port = 8000;
  g_server->onSocketMessage = socketCallback;

  startServer();

  // main infinite loop
  while(1) {

    usleep(5000000); // 5 sec
  }

}
