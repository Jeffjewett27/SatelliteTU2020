/*
 * Author: Isaac Wickham and Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "simpletools.h"
#include "IMUSensor.h"
#include "MagCalibrationThread.h"

int *magCog;

//Starts a separate thread to calibrate the magnetometer
void startMagCalibrationThread() {
  magCog = cog_run(magCalibrationThread, 128);
}

//Runs the thread-blocking function imu_calibrateMagnetometer
void magCalibrationThread() {
  imu_calibrateMagnetometer();
  if (magCog == NULL) {
    return;
  }    
  cog_end(magCog);
}  