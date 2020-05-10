/*
 * Author: Isaac Wickham and Jeff Jewett
 * Modification Date: 5/10/20
*/

//Starts a separate thread to calibrate magnetometer
void startMagCalibrationThread();

//Run the thread-blocking function imu_calibrateMagnetometer
void magCalibrationThread();