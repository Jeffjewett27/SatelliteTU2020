/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

void adc_initI2C();
void adc_setConfig(uint8_t port);
uint16_t readAnalog(uint8_t channel);
void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value);
uint16_t readRegister(uint8_t i2cAddress, uint8_t reg);

float convertAnalog(uint16_t val);