void temperature1_initialize();
void temperature2_initialize();
void temperature3_initialize();

float temperature1_read();  //Temperature from IMU
uint16_t temperature2_read();
uint16_t temperature3_read();

float temperature2_convert(uint16_t val);