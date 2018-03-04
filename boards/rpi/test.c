// gcc test.c -lwiringPi -o test

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <ads1115.h>

#define MY_BASE 2222
#define JOY_RX 0
#define JOY_RY 1
#define POT 2
#define TEMPERATURE 3
#define JOY_BUTT 4

int main ( int argc, char* argv[] ) {
  
  int joy_x, joy_y, pot_val, temp_val, button_press;
  
// sets up the wiringPi library
  if (wiringPiSetupGpio () < 0) {
    puts("ERROR SETTING UP");
    return 1;
  }
  
  ads1115Setup (MY_BASE, 0x48);
  
  while(1) {
    joy_x = analogRead (MY_BASE + JOY_RX);
    joy_y = analogRead (MY_BASE + JOY_RY);
    pot_val = analogRead (MY_BASE + POT);
    temp_val = analogRead (MY_BASE + TEMPERATURE);
    button_press = digitalRead(JOY_BUTT);  
    printf("joy_x: %d\tjoy_y: %d\npot_val: %d\ntemp_val: %d button_press: %d\n", joy_x, joy_y, pot_val, temp_val, button_press);
    sleep(1);
  }
}
