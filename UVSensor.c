#include "adcDCpropab.h"                            // Include adcDCpropAB
#include "simpletools.h"

const float CONVERSIONCONST = 0.1;
		
void uvInit() {
  adc_init(21, 20, 19, 18); //A/D 3,2,1,0
}
  
float uvRead() {
 float analog = adc_volts(1);
 float uvVal = analog/CONVERSIONCONST;
  print("uvreading: %.6f\n", uvVal);
  return uvVal;
}  
