/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "simpletools.h"
#include "TemperatureSensors.h"
#include "IMUSensor.h"
#include "ADC.h"

const uint8_t TEMP2_PIN = 1;

float temperature1_read() {  
  //Temperature from IMU
  return imu_temperatureRead();
}  

uint16_t temperature2_read() {
  uint16_t val = readAnalog(TEMP2_PIN);
  return val;
}  

float temperature2_convert(uint16_t val) {
  float voltage = convertAnalog(val);
  float deltaV = voltage - 0/*1.263*/;
  float converted = deltaV / 0.01 / 1.08789;
  float farh = (converted*9)/5 + 32;
  print("converted: %.2fC, %.2fF\n", converted, farh);
  return converted;
}  