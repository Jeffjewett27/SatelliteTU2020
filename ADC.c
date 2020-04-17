#include "simpleTools.h"
#include "simpleI2C.h"

#define ADS1015_REG_POINTER_MASK (0x03)      ///< Point mask
#define ADS1015_REG_POINTER_CONVERT (0x00)   ///< Conversion
#define ADS1015_REG_POINTER_CONFIG (0x01)    ///< Configuration
#define ADS1015_REG_POINTER_LOWTHRESH (0x02) ///< Low threshold
#define ADS1015_REG_POINTER_HITHRESH (0x03)  ///< High threshold

i2c *adcBus;
const uint16_t CONFIG = 0x0000;
const uint8_t controlByte = 0x48;

void adc_initI2C() {
  //i2c_newBus(scl,sda, mode)
  adc = i2c_newbus(14,  13,   0); //28 and 29 are i2c pin numbers and 0 is an i2c mode
}  

void adc_setConfig() {
  if (adcBus == NULL) {
    adc_initI2C();
  }    

  while(i2c_busy(adcBus, controlByte));
  i2c_out(adcBus, controlByte,                  
          ADS1015_REG_POINTER_CONFIG, 1, &CONFIG, 2); //output the value of pc to EEPROM
}  

uint8_t readAnalog() {                                   
  if (adcBus == NULL) {
    adc_initI2C();
  } 
  
  while(i2c_busy(adcBus, controlByte));
  
  uint16_t val;
  i2c_in(adcBus, controlByte,  
         ADS1015_REG_POINTER_CONVERT, 1, &val, 2); //read the value from EEPROM into pc
  return val;
}