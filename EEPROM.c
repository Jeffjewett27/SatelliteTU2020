/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "simpletools.h"
#include "EEPROM.h"

i2c *eeBus;  // I2C bus ID
const uint8_t checkVal = 183;  //arbitrary nonzero constant value
const uint16_t packetAddr = 32768;  //address in memory of the EEPROM
const uint16_t checkAddrOffset = 16;  //offset above packetAddr
const uint8_t controlByte = 0b1010000;  //grants read/write capabilities

const int EEPROM_SCL = 28;
const int EEPROM_SDIO = 29;

void eeprom_initI2C() {
  eeBus = i2c_newbus(EEPROM_SCL,  EEPROM_SDIO,   0); //28 and 29 are i2c pin numbers and 0 is an i2c mode
}  

void setPacketCount(uint8_t pc) {
  if (eeBus == NULL) {
    eeprom_initI2C();
  }    

  while(i2c_busy(eeBus, controlByte));
  i2c_out(eeBus, controlByte, packetAddr, 2, &pc, 1); //output the value of pc to EEPROM
}

void incrementPacketCount(uint8_t inc) {
  uint8_t val = readPacketCount() + inc;
  setPacketCount(val);
}  

uint8_t readPacketCount() {                                   
  if (eeBus == NULL) {
    eeprom_initI2C();
  } 
  
  while(i2c_busy(eeBus, controlByte));
  
  uint8_t pc;
  i2c_in(eeBus, controlByte, packetAddr, 2, &pc, 1); //read the value from EEPROM into pc
  return pc;
}

void setByteCheck() {
  if (eeBus == NULL) {
    eeprom_initI2C();
  }    

  while(i2c_busy(eeBus, controlByte));
  i2c_out(eeBus, controlByte, packetAddr + checkAddrOffset, 2, &checkVal, 1); //set the byteCheck value into EEPROM
}

void resetByteCheck() {
  if (eeBus == NULL) {
    eeprom_initI2C();
  }    
  uint8_t val = 0;
  while(i2c_busy(eeBus, controlByte));
  i2c_out(eeBus, controlByte, packetAddr + checkAddrOffset, 2, &val, 1); //reset the byteCheck value from EEPROM to 0
}
  
uint8_t isByteCheckSet() {
  if (eeBus == NULL) {
    eeprom_initI2C();
  } 
  
  while(i2c_busy(eeBus, controlByte));
  
  uint8_t bc;
  i2c_in(eeBus, controlByte, packetAddr + checkAddrOffset, 2, &bc, 1); //read the value from EEPROM into bc
  return bc == checkVal; //check if byteCheck is set to checkVal
}