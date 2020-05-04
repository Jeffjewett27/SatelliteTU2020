#include "simpletools.h"
#include "CurrentSenseResistor.h"
#include "ADC.h"

const uint8_t CURRENT_SENSE_ADC = 2;

void currentSenseResistor_initialize() {}

uint16_t currentSenseResistor_read() {
  return readAnalog(CURRENT_SENSE_ADC);
}