#include "simpletools.h"
#include "TemperatureSensors.h"
#include "IMUSensor.h"
#include "ADC.h"

const uint8_t TEMP2_PIN = 1;
const uint8_t TEMP3_PIN = 0;

void temperature1_initialize() {}
void temperature2_initialize() {}
void temperature3_initialize() {}

float temperature1_read() {  
  //Temperature from IMU
  return imu_temperatureRead();
}  

uint16_t temperature2_read() {
  uint16_t val = readAnalog(TEMP2_PIN);
  return val;
}  

uint16_t temperature3_read() {
  uint16_t val = readAnalog(TEMP3_PIN);
  return val;
}  

float temperature2_convert(uint16_t val) {
  float voltage = convertAnalog(val);
  float deltaV = voltage - 0/*1.263*/;
  //print("deltaV: %.2f volts\n", deltaV);
  //float mv = ( converted/1024.0)*5000;
  //float cel = mv/10;
  //float farh = (cel*9)/5 + 32;
  //print("temp: %.2fF or %.2fC\n", farh, cel);
  float converted = deltaV / 0.01 / 1.08789;
  float farh = (converted*9)/5 + 32;
  print("converted: %.2fC, %.2fF\n", converted, farh);
  return converted;
}  