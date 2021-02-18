/*
 * Author: Isaac Wickham and Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "lsm9ds1.h"
#include "IMUSensor.h"
#include "simpletools.h"

int __pinM;
int __mBiasRaw[3] = {0,0,0};
unsigned char __settings_mag_scale;

const int IMU_SCL = 14;
const int IMU_SDIO = 13;
const int IMU_CS_AG = 12;
const int IMU_CS_M = 11;

void imu_initialize() {
  //Parameters: imu_init(scl, sdio, cs_ag, cs_m)
  int val = imu_init(IMU_SCL, IMU_SDIO, IMU_CS_AG, IMU_CS_M);
}
  
//unit: g's
Vector3 imu_accelerometerRead() {
  float _imuX, _imuY, _imuZ;
  Vector3 acceleration;
  imu_readAccelCalculated(&_imuX, &_imuY, &_imuZ);
  acceleration.x = _imuX;
  acceleration.y = _imuY;
  acceleration.z = _imuZ;
  return acceleration;
}  

//units: degrees per second
Vector3 imu_gyroscopeRead() {
  float _imuX, _imuY, _imuZ;
  Vector3 gyroscope;
  imu_readGyroCalculated(&_imuX, &_imuY, &_imuZ);
  gyroscope.x = _imuX;
  gyroscope.y = _imuY;
  gyroscope.z = _imuZ;
  return gyroscope;
}  

//units: degrees per second
Vector3 imu_magnetometerRead() {
  float _imuX, _imuY, _imuZ;
  Vector3 magnet;
  imu_readMagCalculated(&_imuX, &_imuY, &_imuZ);
  magnet.x = _imuX;
  magnet.y = _imuY;
  magnet.z = _imuZ;
  return magnet;
} 

//units: celcius
float imu_temperatureRead() {
  float _temp;
  imu_readTempCalculated(&_temp, 0); //read celcius into _temp
  return _temp;
}