#include "simpletools.h"
#include "ADC.h"
#include "UVSensors.h"

const float UV_CONVERSIONCONST = 0.1; //voltage / UV_CONVERSIONCONST = uv level (will be done in post processing in the SDD)
const uint8_t UV_ADC_PORT = 3;
  
uint16_t uvRead() {
 return readAnalog(UV_ADC_PORT);
}
