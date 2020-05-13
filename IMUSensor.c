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

const int IMU_SCL = 0;
const int IMU_SDIO = 1;
const int IMU_CS_AG = 2;
const int IMU_CS_M = 3;

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

void imu_calibrateMagnetometer() {
  int i = 0, j, mx, my, mz;
  char ck0 = 0, ck1 = 0, ck2 = 0, ck3 = 0, ck4 = 0, ck5 = 0, ck6 = 0, ck7 = 0, ck8 = 0;
  int magMin[3] = {0, 0, 0};
  int magMax[3] = {0, 0, 0}; // The road warrior
  
  float ax, ay, az;
    
  while(i < 128 || (ck0 + ck1 + ck2 + ck3 + ck4 + ck5 + ck6 + ck7 + ck8) < 6)
  {
    while (!imu_magAvailable(ALL_AXIS));
    imu_readMag(&mx, &my, &mz);
    int magTemp[3] = {0, 0, 0};
    magTemp[0] = mx;    
    magTemp[1] = my;
    magTemp[2] = mz;
    for (j = 0; j < 3; j++)
    {
      if (magTemp[j] > magMax[j]) magMax[j] = magTemp[j];
      if (magTemp[j] < magMin[j]) magMin[j] = magTemp[j];
    }

    if(abs(magMax[0] - magMin[0]) > (12000 / ((int) __settings_mag_scale))) ck6 = 1;
    if(abs(magMax[1] - magMin[1]) > (12000 / ((int) __settings_mag_scale))) ck7 = 1;
    if(abs(magMax[2] - magMin[2]) > (12000 / ((int) __settings_mag_scale))) ck8 = 1;

    imu_readAccelCalculated(&ax, &ay, &az);
    if(ax > 0.85 && ay < 0.15 && ay > -0.15 && az < 0.15 && az > -0.15) ck0 = 1;
    if(ax < -0.85 && ay < 0.15 && ay > -0.15 && az < 0.15 && az > -0.15) ck1 = 1;
    if(ay > 0.85 && ax < 0.15 && ax > -0.15 && az < 0.15 && az > -0.15) ck2 = 1;
    if(ay < -0.85 && ax < 0.15 && ax > -0.15 && az < 0.15 && az > -0.15) ck3 = 1;
    if(az > 0.85 && ay < 0.15 && ay > -0.15 && ax < 0.15 && ax > -0.15) ck4 = 1;
    if(az < -0.85 && ay < 0.15 && ay > -0.15 && ax < 0.15 && ax > -0.15) ck5 = 1;
    
    i++;
    
    if (i > 10000) {
      //has looped for at least 100 seconds
      break;
    }      
    pause(10);
  }

  for (j = 0; j < 3; j++)
  {
    __mBiasRaw[j] = (magMax[j] + magMin[j]) / 2;
  }
  
  unsigned char msb, lsb;
  for(int k = 0; k < 3; k++)
  {
    msb = (__mBiasRaw[k] & 0xFF00) >> 8;
    lsb = __mBiasRaw[k] & 0x00FF;
    imu_SPIwriteByte(__pinM, OFFSET_X_REG_L_M + (2 * k), lsb);
    imu_SPIwriteByte(__pinM, OFFSET_X_REG_H_M + (2 * k), msb);
  }
}