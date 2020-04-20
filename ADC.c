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

void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
  i2cAddress <<= 1;
  i2cAddress &= -2;
  i2c_start(adcBus);
  if(i2c_writeByte(busID, i2cAddr)) return;
  //Wire.beginTransmission(i2cAddress);
  i2c_writeByte(adcBus, reg);
  //i2cwrite((uint8_t)reg);
  i2c_writeByte(adcBus, (uint8_t)(value >> 8));
  //i2cwrite((uint8_t)(value >> 8));
  i2c_writeByte(adcBus, (uint8_t)(value & 0xFF));
  //i2cwrite((uint8_t)(value & 0xFF));
  i2c_stop(adcBus);
  //Wire.endTransmission();
}

uint16_t readRegister(uint8_t i2cAddress, uint8_t reg) {
  i2cAddress <<= 1;
  i2cAddress &= -2;
  i2c_start(adcBus);
  if(i2c_writeByte(busID, i2cAddress)) return 0;
  //Wire.beginTransmission(i2cAddress);
  i2c_writeByte(adcBus, reg);
  //i2cwrite(reg);
  i2c_stop(adcBus);
  //Wire.endTransmission();
  i2cAddress |= 1;                                       // Set i2cAddr.bit0 (read)
  i2c_start(adcBus);
  if(i2c_writeByte(adcBus, i2cAddress)) return 0;
  uint8_t readBuf[2];
  readBuf[0] = i2c_readByte(adcBus, 0);
  readBuf[1] = i2c_readByte(adcBus, 1);
  //Wire.requestFrom(i2cAddress, (uint8_t)2);
  return ((buf[0] << 8) | buf[1]);
}