/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#ifndef SENSOR_READINGS
#define SENSOR_READINGS

#include "Vector3.h"

#define NUM_1_BYTE_READINGS 32
#define NUM_2_BYTE_READINGS 16
#define NUM_4_BYTE_READINGS 8

//a struct to store all the readings being collected
struct SensorReadings {
  Vector3 accelerationReadings[NUM_2_BYTE_READINGS];
  Vector3 gyroscopeReadings[NUM_2_BYTE_READINGS];
  Vector3 magnetometerReadings[NUM_2_BYTE_READINGS];
  uint16_t uv1Readings[NUM_2_BYTE_READINGS];
  float temp1Readings[NUM_2_BYTE_READINGS];
  uint16_t temp2Readings[NUM_2_BYTE_READINGS];
  uint16_t currentSenseReadings[NUM_2_BYTE_READINGS];
  uint16_t lightToFrequencyReadings[NUM_2_BYTE_READINGS];
  uint16_t gammaReadings[NUM_1_BYTE_READINGS];
};

typedef struct SensorReadings SensorReadings;

#endif