/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "simpleTools.h"
#include "simpleI2C.h"
#include "ADC.h"

#define ADS1015_REG_POINTER_MASK (0x03)      ///< Point mask
#define ADS1015_REG_POINTER_CONVERT (0x00)   ///< Conversion
#define ADS1015_REG_POINTER_CONFIG (0x01)    ///< Configuration
#define ADS1015_REG_POINTER_LOWTHRESH (0x02) ///< Low threshold
#define ADS1015_REG_POINTER_HITHRESH (0x03)  ///< High threshold

i2c *adcBus;
#ifndef ADC_CONSTANTS
#define ADC_CONSTANTS
 uint16_t CONFIG = 0b1100000110000011;
 uint8_t adcAddress = 0x48;
#endif

const int ADC_SCL = 14;
const int ADC_SDA = 13;


void adc_initI2C() {
  //Parameters: i2c_newBus(scl, sda, mode)
  adcBus = i2c_newbus(ADC_SCL, ADC_SDA, 0);
}  

void adc_setConfig(uint8_t port) {
  if (adcBus == NULL) {
    adc_initI2C();
  }

  while(i2c_busy(adcBus, adcAddress));
  uint16_t config = CONFIG | (port << 12);
  writeRegister(adcAddress, ADS1015_REG_POINTER_CONFIG, config);
}  

uint16_t readAnalog(uint8_t channel) {                                   
  if (adcBus == NULL) {
    adc_initI2C();
  } 
  
  while(i2c_busy(adcBus, adcAddress));
  adc_setConfig(channel);
  pause(12);
  
  uint16_t val = readRegister(adcAddress, ADS1015_REG_POINTER_CONVERT);
  return val;
}

void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
  i2cAddress <<= 1;
  i2cAddress &= -2;
  i2c_start(adcBus);
  if(i2c_writeByte(adcBus, i2cAddress)) return;
  i2c_writeByte(adcBus, reg);
  
  i2c_writeByte(adcBus, (uint8_t)(value >> 8));
  i2c_writeByte(adcBus, (uint8_t)(value & 0xFF));
  i2c_stop(adcBus);
}

uint16_t readRegister(uint8_t i2cAddress, uint8_t reg) {
  i2cAddress <<= 1;
  i2cAddress &= -2;
  i2c_start(adcBus);
  if(i2c_writeByte(adcBus, i2cAddress)) return 0;
  i2c_writeByte(adcBus, reg);
  i2c_stop(adcBus);
  i2cAddress |= 1;  // Set i2cAddr.bit0 (read)
  i2c_start(adcBus);
  if(i2c_writeByte(adcBus, i2cAddress)) return 0;
  uint8_t readBuf[2];
  readBuf[0] = i2c_readByte(adcBus, 0);
  readBuf[1] = i2c_readByte(adcBus, 1);
  return ((readBuf[0] << 8) | readBuf[1]);
}

float convertAnalog(uint16_t x) {
  float m = 0.00018750;
  float b = 0.0024810;
  float converted = m*x + b;
  return converted;
}  