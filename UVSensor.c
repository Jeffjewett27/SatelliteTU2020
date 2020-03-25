#include "adcDCpropab.h"                            // Include adcDCpropAB
#include "simpletools.h"

void uvInit() {
  adc_init(21, 20, 19, 18); //A/D 3,2,1,0
}
  
float uvRead() {
  int uvInput = input(1);
  float analog = adc_volts(1);
  print("uvreading: %.6f\n", analog);
  return analog;
}  
