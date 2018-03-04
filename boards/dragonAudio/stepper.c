#include <stdio.h>
#include <unistd.h>
#include "gpio.h"

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

int main(int argc, char* argv[]) {

  GpioOutput(A1, 0);
  GpioOutput(A2, 0);
  GpioOutput(B1, 0);
  GpioOutput(B2, 0);

  forward(atoi(argv[1]), atoi(argv[2]));
}
