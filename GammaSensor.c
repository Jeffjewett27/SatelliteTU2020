#include "simpletools.h"
#include "GammaSensor.h"

const int GAMMA_PIN = 0;
const int LIGHT_PIN = 2;
void gamma_initialize() {}

uint8_t gamma_getPulseCount() {
  int gCount = 0;
  int lCount = 0;
  int gFreeze = 0;
  int lFreeze = 0;
  while (1) {
    if (!gFreeze && input(GAMMA_PIN)) {
      print("gamma: %d\n", gCount++);
      gFreeze = 1;
    } else if (!input(GAMMA_PIN)) {
      gFreeze = 0;
    }
    
    if (!gFreeze && input(LIGHT_PIN)) {
      if (lCount % 1000 == 0) {
        print("light: %d\n", lCount);
      }      
      lCount++;
      lFreeze = 1;
    } else if (!input(LIGHT_PIN)) {
      lFreeze = 0;
    }
  }    
}  