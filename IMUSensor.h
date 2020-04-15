#include "Vector3.h"

Vector3 imu_accelerometerRead();
Vector3 imu_magnetometerRead();
Vector3 imu_gyroscopeRead();
void imu_initialize(); 
void imu_calibrateMagnetometer();