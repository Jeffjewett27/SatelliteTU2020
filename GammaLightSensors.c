#include "simpletools.h"
#include "GammaSensor.h"

const int GAMMA_PIN = 0;
const int LIGHT_PIN = 2;

volatile uint32_t gammaCount = 0;
volatile uint32_t lightCount = 0;
volatile int gOverflow = 0;
volatile int lOverflow = 0;
volatile int gReset = 0;
volatile int lReset = 0;

int *lgCog;

void startLightGammaThread() {
  lgCog = cog_run(gamma_light_thread, 128);
}

void stopLightGammaThread() {
  cog_end(lgCog);
}  
  
void gamma_initialize() {
  gammaCount = 0;
  gOverflow = 0;
  gReset = 0;
}

void light_initialize() {
  lightCount = 0;
  lOverflow = 0;
  lReset = 0;
}

uint8_t gamma_light_thread() {
  int gFreeze = 0;
  int lFreeze = 0;
  gamma_initialize();
  light_initialize();
  while (1) {
    if (gReset) {
      gamma_initialize();
    }      
    if (gFreeze) {
      gFreeze = 0;
    } else if (input(GAMMA_PIN)) {
      print("gamma: %d\n", gCount);
      
      gCount++;
      if (gCount == 0) {
        //an overflow has happened
        gOverflow++; 
      }        
      gFreeze = 1;
    }            
    
    if (lReset) {
      light_initialize();
    }      
    if (lFreeze) {
      lFreeze = 0;
    } else if (input(LIGHT_PIN)) {
      if (lCount % 1000 == 0) {
        print("light: %d\n", lCount);
      }      
      lCount++;
      if (lCount == 0) {
        //an overflow has happened
        lOverflow++; 
      }  
      lFreeze = 1;
    }
  }    
}  

uint32_t gamma_read_reset() {
  uint32_t val = gCount;
  if (gOverflow) {
    //if there was an overflow, return max value instead of some lower amount
    val = ~0; //max value for unsigned int
  }
  gReset = 1; //let the other thread take care of resetting to avoid race condition
  return val;
}  

uint32_t lightToFrequency_read_reset() {
  uint32_t val = lCount;
  if (lOverflow) {
    //if there was an overflow, return max value instead of some lower amount
    val = ~0; //max value for unsigned int
  }
  lReset = 1; //let the other thread take care of resetting to avoid race condition
  return val;
}  

uint32_t gamma_read() {
  uint32_t val = gCount;
  if (gOverflow) {
    //if there was an overflow, return max value instead of some lower amount
    val = ~0; //max value for unsigned int
  }
  return val;
}  

uint32_t lightToFrequency_read() {
  uint32_t val = lCount;
  if (lOverflow) {
    //if there was an overflow, return max value instead of some lower amount
    val = ~0; //max value for unsigned int
  }
  return val;
}  