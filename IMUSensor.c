#include "lsm9ds1.h"
#include "IMUSensor.h"

void imu_initialize() {
  imu_init(7, 6, 5, 4);
}
  
//unit: g's
struct Vector3 imu_accelerometerRead() {
  float _imuX, _imuY, _imuZ;
  struct Vector3 acceleration;
  imu_readAccelCalculated(&_imuX, &_imuY, &_imuZ);
  acceleration.x = _imuX;
  acceleration.y = _imuY;
  acceleration.z = _imuZ;
  return acceleration;
}  

//units: degrees per second
struct Vector3 imu_gyroscopeRead() {
  float _imuX, _imuY, _imuZ;
  struct Vector3 gyroscope;
  imu_readGyroCalculated(&_imuX, &_imuY, &_imuZ);
  gyroscope.x = _imuX;
  gyroscope.y = _imuY;
  gyroscope.z = _imuZ;
  return gyroscope;
}  

//units: degrees per second
struct Vector3 imu_magnetometerRead() {
  float _imuX, _imuY, _imuZ;
  struct Vector3 magnet;
  imu_readMagCalculated(&_imuX, &_imuY, &_imuZ);
  magnet.x = _imuX;
  magnet.y = _imuY;
  magnet.z = _imuZ;
  return magnet;
} 

void imu_calibrateMagnetometer() {
  high(26);
  high(27);
  imu_calibrateMag();
  low(26);
  low(27);
}