#include "simpletools.h"
#include "IMUSensor.h"

int *magCog;

void magCalibrationThread();

void startMagCalibrationThread() {
  print("start");
  magCog = cog_run(magCalibrationThread, 128);
}

void magCalibrationThread() {
  high(27);
  imu_calibrateMagnetometer();
  if (magCog == NULL) {
    return;
  }    
  low(27);
  cog_end(magCog);
}  