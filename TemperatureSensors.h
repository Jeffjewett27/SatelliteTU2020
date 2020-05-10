//Temperature from IMU
float temperature1_read();  

//Temperature through ADC
uint16_t temperature2_read();

//Post processing- ADC val to temperature
float temperature2_convert(uint16_t val);