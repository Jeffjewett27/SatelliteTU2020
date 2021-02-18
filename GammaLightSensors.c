#include "simpletools.h"
#include "GammaLightSensors.h"

#define UINT16T_MAX 0xffff
#define UINT8T_MAX 0xff

const int LTF_PIN = 2;  //Light to frequency
const int RAD_PIN = 0;  //Radiation (particle detector)

volatile uint32_t ltfCount = 0;
volatile int ltfOverflow = 0;
volatile int ltfReset = 0;
int *ltfCog;

volatile uint16_t radCount = 0; 
volatile int radOverflow = 0; 
volatile int radReset = 0;
int *radCog;


void startLtfThread() {
  ltfCog = cog_run(ltfThread, 128);
}

void stopLtfThread() {
  cog_end(ltfCog);
}  

void ltfThread() {
  while(1) {
    if (ltfReset) {
      initLtf();
    }
    int temp = ltfCount;
    temp += count(LTF_PIN, 10);
    if (temp < ltfCount) {
      ltfOverflow = 1;
    }
    ltfCount = temp;
  }    
}  
  
void initLtf() {
  ltfCount = 0;
  ltfOverflow = 0;
  ltfReset = 0;
}

uint16_t lightToFrequency_read_reset() {
  uint16_t val = ltfOverflow ? UINT16T_MAX : (ltfCount >> 4) & 0xffff;
  ltfCount = 0;
  ltfReset = 1;
  return val;
}

void startGammaThread() {
  radCog = cog_run(radThread, 128);
}

void stopRadThread() {
  cog_end(radCog);
}  

void radThread() {
  while(1) {
    if (radReset) {
      initRad();
    }
    int temp = radCount;
    temp += count(RAD_PIN, 10);
    if (temp > UINT8T_MAX) {
      radOverflow = 1;
    }
    radCount = temp;
  }    
}  
  
void initRad() {
  radCount = 0;
  radOverflow = 0;
  radReset = 0;
}

uint16_t gamma_read_reset() {
  uint16_t val = radCount;
  radCount = 0;
  radReset = 1;
  return val;
}
