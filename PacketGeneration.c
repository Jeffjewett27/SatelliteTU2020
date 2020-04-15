#include "PacketGeneration.h"
#include "DataConversion.h"

#define NUM_1_BYTE_READINGS 32
#define NUM_2_BYTE_READINGS 16
#define NUM_4_BYTE_READINGS 8

const uint8_t MAGX_FN = 0x44;
const uint8_t MAGY_FN = 0x45;
const uint8_t MAGZ_FN = 0x46;
const uint8_t GYROX_FN = 0x4B;
const uint8_t GYROY_FN = 0x4C;
const uint8_t GYROZ_FN = 0x4D;
const uint8_t ACCX_FN = 0x4E;
const uint8_t ACCY_FN = 0x4F;
const uint8_t ACCZ_FN = 0x40;
const uint8_t UV1_FN = 0x43;

const uint8_t MAGX_FN_COMP = 0x24;
const uint8_t MAGY_FN_COMP = 0x25;
const uint8_t MAGZ_FN_COMP = 0x26;
const uint8_t GYROX_FN_COMP = 0x2B;
const uint8_t GYROY_FN_COMP = 0x2C;
const uint8_t GYROZ_FN_COMP = 0x2D;
const uint8_t ACCX_FN_COMP = 0x2E;
const uint8_t ACCY_FN_COMP = 0x2F;
const uint8_t ACCZ_FN_COMP = 0x20;
const uint8_t UV1_FN_COMP = 0x23;

const uint8_t GENERAL_FN = 0x01;

void setPacketFields(Packet *sensorPacket, uint8_t fnCode, uint8_t iteration, uint8_t packetsCounter) {
  sensorPacket->fnCode = fnCode;
  sensorPacket->iteration = iteration;
  sensorPacket->packetsCounter = packetsCounter;
}  

uint16_t compressAccelerometer(float accel) {
  return reduceFloat16bit(accel, 1, 5);
}

uint16_t compressGyroscope(float gyro) {
  return reduceFloat16bit(gyro, 1, 5);
}    

uint16_t compressMagnetometer(float mag) {
  return reduceFloat16bit(mag, 1, 5);
}    

uint16_t compressUV(float uv) {
  return reduceFloat16bit(uv, 0, 5);
}  

Packet generateGeneralSensorPacket(uint8_t iteration, uint8_t packetsCounter, int i, Vector3 *accReads, 
              Vector3 *magReads, Vector3 *gyroReads, float *uvReads) {
  Packet generalSensorPacket;
  //Create "general sensor values" packets, both compressed and uncompressed.
  //Add data:
  //Acceleration readings
  generalSensorPacket.ArrayType.twoByte[0] = compressAccelerometer(accReads[i/2].x);
  generalSensorPacket.ArrayType.twoByte[2] = compressAccelerometer(accReads[i/2].y);
  generalSensorPacket.ArrayType.twoByte[4] = compressAccelerometer(accReads[i/2].z);
  //Gyroscope readings
  generalSensorPacket.ArrayType.twoByte[6] = compressGyroscope(gyroReads[i/2].x);
  generalSensorPacket.ArrayType.twoByte[8] = compressGyroscope(gyroReads[i/2].y);
  generalSensorPacket.ArrayType.twoByte[10] = compressGyroscope(gyroReads[i/2].z);
  //Magnetometer readings
  generalSensorPacket.ArrayType.twoByte[12] = compressMagnetometer(magReads[i/2].x);
  generalSensorPacket.ArrayType.twoByte[14] = compressMagnetometer(magReads[i/2].y);
  generalSensorPacket.ArrayType.twoByte[16] = compressMagnetometer(magReads[i/2].z);
  //UV reading
  generalSensorPacket.ArrayType.twoByte[18] = compressUV(uvReads[i/2]);
  setPacketFields(&generalSensorPacket, GENERAL_FN, iteration, packetsCounter);
  return generalSensorPacket;
}  

Packet generateMagX(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter) {
  //Un-compressed
  Packet sensorPacket;
  for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.fourByte[i] = magReads[i*2].x;
  }
  setPacketFields(&sensorPacket, MAGX_FN, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateMagXCompressed(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter) {
  //Compressed  
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = compressMagnetometer(magReads[i].x);
  }
  setPacketFields(&sensorPacket, MAGX_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateMagY(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter) {
  //Un-compressed
  Packet sensorPacket;
  for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.fourByte[i] = magReads[i*2].y;
  }
  setPacketFields(&sensorPacket, MAGY_FN, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateMagYCompressed(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter) {
  //Compressed  
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = compressMagnetometer(magReads[i].y);
  }
  setPacketFields(&sensorPacket, MAGY_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateMagZ(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter) {
  //Un-compressed
  Packet sensorPacket;
  for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.fourByte[i] = magReads[i*2].z;
  }
  setPacketFields(&sensorPacket, MAGZ_FN, iteration, packetsCounter);
  return sensorPacket;
}

Packet generateMagZCompressed(Vector3 *magReads, uint8_t iteration, uint8_t packetsCounter) {
  //Compressed  
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = compressMagnetometer(magReads[i].z);
  }
  setPacketFields(&sensorPacket, MAGZ_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateGyroX(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter) {
  //Un-compressed
  Packet sensorPacket;
  for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.fourByte[i] = gyroReads[i*2].x;
  }
  setPacketFields(&sensorPacket, GYROX_FN, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateGyroXCompressed(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter) {
  //Compressed  
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = compressGyroscope(gyroReads[i].x);
  }
  setPacketFields(&sensorPacket, GYROX_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateGyroY(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter) {
  //Un-compressed
  Packet sensorPacket;
  for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.fourByte[i] = gyroReads[i*2].y;
  }
  setPacketFields(&sensorPacket, GYROY_FN, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateGyroYCompressed(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter) {
  //Compressed  
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = compressGyroscope(gyroReads[i].y);
  }
  setPacketFields(&sensorPacket, GYROZ_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateGyroZ(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter) {
  //Un-compressed
  Packet sensorPacket;
  for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.fourByte[i] = gyroReads[i*2].z;
  }
  setPacketFields(&sensorPacket, GYROZ_FN, iteration, packetsCounter);
  return sensorPacket;
}

Packet generateGyroZCompressed(Vector3 *gyroReads, uint8_t iteration, uint8_t packetsCounter) {
  //Compressed  
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = compressGyroscope(gyroReads[i].z);
  }
  setPacketFields(&sensorPacket, GYROZ_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateAccX(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter) {
  //Un-compressed
  Packet sensorPacket;
  for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.fourByte[i] = accReads[i*2].x;
  }
  setPacketFields(&sensorPacket, ACCX_FN, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateAccXCompressed(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter) {
  //Compressed  
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = compressAccelerometer(accReads[i].x);
  }
  setPacketFields(&sensorPacket, ACCX_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateAccY(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter) {
  //Un-compressed
  Packet sensorPacket;
  for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.fourByte[i] = accReads[i*2].y;
  }
  setPacketFields(&sensorPacket, ACCY_FN, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateAccYCompressed(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter) {
  //Compressed  
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = compressAccelerometer(accReads[i].y);
  }
  setPacketFields(&sensorPacket, ACCY_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateAccZ(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter) {
  //Un-compressed
  Packet sensorPacket;
  for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.fourByte[i] = accReads[i*2].y;
  }
  setPacketFields(&sensorPacket, ACCZ_FN, iteration, packetsCounter);
  return sensorPacket;
}

Packet generateAccZCompressed(Vector3 *accReads, uint8_t iteration, uint8_t packetsCounter) {
  //Compressed  
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = compressAccelerometer(accReads[i].z);
  }
  setPacketFields(&sensorPacket, ACCZ_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateUV(float *uvReads, uint8_t iteration, uint8_t packetsCounter) {
  //Un-compressed
  Packet sensorPacket;
  for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.fourByte[i] = uvReads[i*2];
  }
  setPacketFields(&sensorPacket, UV1_FN, iteration, packetsCounter);
  return sensorPacket;
}

Packet generateUVCompressed(float *uvReads, uint8_t iteration, uint8_t packetsCounter) {
  //Compressed  
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = compressUV(uvReads[i]);
  }
  setPacketFields(&sensorPacket, UV1_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}  