#include "Vector3.h"

struct Vector3 imu_accelerometerRead();
struct Vector3 imu_magnetometerRead();
struct Vector3 imu_gyroscopeRead();
void imu_initialize(); 
void imu_calibrateMagnetometer();