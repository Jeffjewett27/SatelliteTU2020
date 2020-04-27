void adc_initI2C();
void adc_setConfig(uint8_t port);
uint16_t readAnalog(uint8_t channel);
void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value);
uint16_t readRegister(uint8_t i2cAddress, uint8_t reg);
uint16_t readConfig();

float convertAnalog(uint16_t val);