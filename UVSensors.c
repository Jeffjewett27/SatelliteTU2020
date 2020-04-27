#include "simpletools.h"
#include "ADC.h"
#include "UVSensors.h"

const float UV1_CONVERSIONCONST = 0.1; //voltage / UV1_CONVERSIONCONST = uv level (will be done in post processing in the SDD)
const uint8_t UV1_ADC_PORT = 0;
const uint8_t UV2_ADC_PORT = 1;
  
uint16_t uv1Read() {
 return readAnalog(UV1_ADC_PORT);
}  

uint16_t uv2Read() {
  return readAnalog(UV2_ADC_PORT);
}
