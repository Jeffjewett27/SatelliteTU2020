void temperature1_initialize();
void temperature2_initialize();

float temperature1_read();  //Temperature from IMU
uint16_t temperature2_read();

float temperature2_convert(uint16_t val);