/*
 * Author: Jeff Jewett and Mitchell Toth
 * Modification Date: 5/10/20
*/

#include "Packet.h"
#include "Vector3.h"
#include "SensorReadings.h"

void setPacketFields(Packet *sensorPacket, uint8_t fnCode, uint8_t iteration, uint8_t packetsCounter);

//compress float values
uint16_t compressAccelerometer(float accel);
uint16_t compressGyroscope(float gyro);
uint16_t compressMagnetometer(float mag);
uint16_t compressIMUTemp(float temp);

Packet generateGeneralSensorPacket(uint8_t iteration, uint8_t packetsCounter, int i, SensorReadings *sensors);
              
Packet generateMagX(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateMagY(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateMagZ(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateMagXCompressed(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateMagYCompressed(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateMagZCompressed(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateGyroX(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateGyroY(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateGyroZ(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateGyroXCompressed(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateGyroYCompressed(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateGyroZCompressed(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateAccX(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateAccY(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateAccZ(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateAccXCompressed(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateAccYCompressed(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateAccZCompressed(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateUV1(uint16_t *uv1Reads, uint8_t iteration, uint8_t packetsCounter);

Packet generateTemp1(float *temp1Reads, uint8_t iteration, uint8_t packetsCounter);
Packet generateTemp1Compressed(float *temp1Reads, uint8_t iteration, uint8_t packetsCounter);
Packet generateTemp2(uint16_t *temp2Reads, uint8_t iteration, uint8_t packetsCounter);

Packet generateLightToFrequency(uint16_t *lightToFrequencyReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateGamma(uint16_t *gammaReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateGammaComp(uint16_t *gammaReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateCurrentSenseResistor(uint16_t *ambientLightReads, uint8_t iteration, uint8_t packetsCounter);