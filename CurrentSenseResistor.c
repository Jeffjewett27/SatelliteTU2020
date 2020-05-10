/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "simpletools.h"
#include "CurrentSenseResistor.h"
#include "ADC.h"

const uint8_t CURRENT_SENSE_ADC = 2;

uint16_t currentSenseResistor_read() {
  return readAnalog(CURRENT_SENSE_ADC);
}