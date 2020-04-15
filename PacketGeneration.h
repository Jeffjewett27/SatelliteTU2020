#include "Packet.h"
#include "Vector3.h"

void setPacketFields(Packet *sensorPacket, uint8_t fnCode, uint8_t iteration, uint8_t packetsCounter);

uint16_t compressAccelerometer(float accel);
uint16_t compressGyroscope(float gyro);
uint16_t compressMagnetometer(float mag);
uint16_t compressUV(float uv);

Packet generateGeneralSensor(uint8_t iteration, uint8_t packetsCounter, int i, Vector3 *accReads, 
              Vector3 *magReads, Vector3 *gyroReads, float *uvReads);
              
Packet generateMagXSensor(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateMagYSensor(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateMagZSensor(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateMagXCompressed(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateMagYCompressed(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateMagZCompressed(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateGyroXSensor(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateGyroYSensor(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateGyroZSensor(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateGyroXCompressed(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateGyroYCompressed(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateGyroZCompressed(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateAccXSensor(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateAccYSensor(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateAccZSensor(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);

Packet generateAccXCompressed(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateAccYCompressed(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);
Packet generateAccZCompressed(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter);

