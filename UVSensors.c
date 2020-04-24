#include "adcDCpropab.h"                            // Include adcDCpropAB
#include "simpletools.h"

const float CONVERSIONCONST = 0.1;
		
//--- UV Sensor 1 ---
void uv1Init() {
  adc_init(21, 20, 19, 18); //A/D 3,2,1,0
}
  
float uv1Read() {
 float analog = adc_volts(1);
 float uvVal = analog/CONVERSIONCONST;
  print("uvreading: %.6f\n", uvVal);
  return uvVal;
}  


//--- UV Sensor 2 ---
void uv2Init() {}

uint16_t uv2Read() {
  return 0;
}
