/*
  Test 24LC512 with I2C.c
  Test writes data to I2C EEPROM, then reads it back and displays it.   
*/

#include "simpletools.h"                      // Include simpletools header

i2c *eeBus;                                   // I2C bus ID
uint8_t packetAddr = 32768;
uint8_t controlByte = 0b1010000;


void eeprom_initI2C() {
  eeBus = i2c_newbus(28,  29,   0);
}  

void setPacketCount(uint8_t pc)                                    // Main function
{
  if (eeBus == NULL) {
    eeprom_initI2C();
  }    

  while(i2c_busy(eeBus, controlByte));
  i2c_out(eeBus, controlByte,                  
          packetAddr, 2, &pc, 1);        
}

uint8_t readPacketCount() {                                   
  if (eeBus == NULL) {
    eeprom_initI2C();
  } 
  
  while(i2c_busy(eeBus, controlByte));
  
  uint8_t pc;
  i2c_in(eeBus, controlByte,  
         packetAddr, 2, &pc, 1);
  return pc;
}
