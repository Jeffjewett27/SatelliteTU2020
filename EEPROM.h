/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

//initializes the i2c connection
void eeprom_initI2C();

//stores pc in the eeprom
void setPacketCount(uint8_t pc);

void incrementPacketCount(uint8_t inc);

//reads pc from eeprom
uint8_t readPacketCount();  

//sets a unique value in eeprom
void setByteCheck();

//resets unique value to 0 in eeprom (use before sending into space)
void resetByteCheck();

//checks if unique value has been set
uint8_t isByteCheckSet();
