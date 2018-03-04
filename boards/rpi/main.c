// gcc test.c -lwiringPi -o test
#include "WebSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <ads1115.h>
#include <math.h>

#define MY_BASE 2222
#define JOY_RX 0
#define JOY_RY 1
#define POT 2
#define TEMPERATURE 3
#define JOY_BUTT 4

#define UPPER_T 31000
#define LOWER_T 3000

int main ( int argc, char* argv[] ) {

  const int B = 4275;
  const int R0 = 100000;
  int joy_x, joy_y, pot_val, temp_val, button_press;
  int t_x, t_y;
  char* data_m;

  data_m = (char*)malloc(sizeof(char) * 1024);
  if (data_m ==NULL) { puts("MALLOC FAUL"); }
  
  WebSocket ws;

  std::string IP("192.168.43.105");
  int PORT = 5000;
  
  ws.connectSocket(IP, PORT);
  
// sets up the wiringPi library
  if (wiringPiSetupGpio () < 0) {
    puts("ERROR SETTING UP");
    return 1;
  }
  
 ads1115Setup (MY_BASE, 0x48);
  
  while(1) {
    t_x = analogRead(MY_BASE + JOY_RX) ;
    t_y = analogRead (MY_BASE + JOY_RY);

    if (t_x > UPPER_T) { joy_x = 1; }
    else if (t_x < LOWER_T) { joy_x = -1; }
    else { joy_x = 0; }
    
    if (t_y > UPPER_T) { joy_y = 1; }
    else if (t_y < LOWER_T) { joy_y = -1; }
    else { joy_y = 0; }
    
    pot_val = analogRead (MY_BASE + POT)/1261;
    temp_val = analogRead (MY_BASE + TEMPERATURE)/275;
    button_press = !digitalRead(JOY_BUTT);
    
    sprintf(data_m, "8:%d,%d,%d,%d,%d", joy_x, joy_y, button_press, pot_val, temp_val);

    printf("%s\n", data_m);
      
    ws.send_c(data_m);
	    
    usleep(500000); //300ms
  }
}
