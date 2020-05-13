/*
 * Author: Jeff Jewett and Mitchell Toth
 * Modification Date: 5/10/20
*/

#include "PacketGeneration.h"
#include "DataConversion.h"

#define NUM_1_BYTE_READINGS 32
#define NUM_2_BYTE_READINGS 16
#define NUM_4_BYTE_READINGS 8

const uint8_t GENERAL_FN = 0x01;

const uint8_t GAMMA_FN = 0x50;
const uint8_t GAMMA_FN_COMP = 0x2B;

const uint8_t MAGX_FN_COMP = 0x20;
const uint8_t MAGY_FN_COMP = 0x21;
const uint8_t MAGZ_FN_COMP = 0x22;
const uint8_t TEMP1_FN_COMP = 0x23;
const uint8_t LIGHT_TO_FREQUENCY_FN_COMP = 0x24;
const uint8_t GYROX_FN_COMP = 0x25;
const uint8_t GYROY_FN_COMP = 0x26;
const uint8_t GYROZ_FN_COMP = 0x27;
const uint8_t ACCX_FN_COMP = 0x28;
const uint8_t ACCY_FN_COMP = 0x29;
const uint8_t ACCZ_FN_COMP = 0x2A;

const uint8_t CURRENT_SENSE_RESISTOR_FN = 0x41;
const uint8_t MAGX_FN = 0x42;
const uint8_t MAGY_FN = 0x43;
const uint8_t MAGZ_FN = 0x44;
const uint8_t TEMP1_FN = 0x45;
const uint8_t TEMP2_FN = 0x46;
const uint8_t TEMP3_FN = 0x47;
const uint8_t LIGHT_TO_FREQUENCY_FN = 0x48;
const uint8_t GYROX_FN = 0x49;
const uint8_t GYROY_FN = 0x4A;
const uint8_t GYROZ_FN = 0x4B;
const uint8_t ACCX_FN = 0x4C;
const uint8_t ACCY_FN = 0x4D;
const uint8_t ACCZ_FN = 0x4E;
const uint8_t UV1_FN = 0x4F;




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

uint16_t compressIMUTemp(float temp) {
  return reduceFloat16bit(temp, 0, 5);
}  

uint16_t compressLightToFrequency(float ltf) {
  return reduceFloat16bit(ltf, 0, 5);
}

Packet generateGeneralSensorPacket(uint8_t iteration, uint8_t packetsCounter, int i, SensorReadings *sensors) {
  Packet generalSensorPacket;
  //Acceleration readings
  generalSensorPacket.ArrayType.twoByte[0] = compressAccelerometer(sensors->accelerationReadings[i/2].x);
  generalSensorPacket.ArrayType.twoByte[2] = compressAccelerometer(sensors->accelerationReadings[i/2].y);
  generalSensorPacket.ArrayType.twoByte[4] = compressAccelerometer(sensors->accelerationReadings[i/2].z);
  //Gyroscope readings
  generalSensorPacket.ArrayType.twoByte[6] = compressGyroscope(sensors->gyroscopeReadings[i/2].x);
  generalSensorPacket.ArrayType.twoByte[8] = compressGyroscope(sensors->gyroscopeReadings[i/2].y);
  generalSensorPacket.ArrayType.twoByte[10] = compressGyroscope(sensors->gyroscopeReadings[i/2].z);
  //Magnetometer readings
  generalSensorPacket.ArrayType.twoByte[12] = compressMagnetometer(sensors->magnetometerReadings[i/2].x);
  generalSensorPacket.ArrayType.twoByte[14] = compressMagnetometer(sensors->magnetometerReadings[i/2].y);
  generalSensorPacket.ArrayType.twoByte[16] = compressMagnetometer(sensors->magnetometerReadings[i/2].z);
  //UV readings
  generalSensorPacket.ArrayType.twoByte[18] = compressUV(sensors->uv1Readings[i/2]);
  //Temperature readings
  generalSensorPacket.ArrayType.fourByte[20] = sensors->temp1Readings[i/2]; //4 byte value- uncompressed
  generalSensorPacket.ArrayType.twoByte[24] = sensors->temp2Readings[i/2];
  //Current Sense reading
  generalSensorPacket.ArrayType.twoByte[26] = sensors->currentSenseReadings[i/2];
  //Light to Frequency reading
  generalSensorPacket.ArrayType.twoByte[28] = compressLightToFrequency(sensors->lightToFrequencyReadings[i/2]);
  generalSensorPacket.ArrayType.oneByte[30] = sensors->gammaReadings[i];
  
  //byte 31 unset
  
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

Packet generateUV1(uint16_t *uv1Reads, uint8_t iteration, uint8_t packetsCounter) {
  //Un-compressed
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = uv1Reads[i];
  }
  setPacketFields(&sensorPacket, UV1_FN, iteration, packetsCounter);
  return sensorPacket;
}

Packet generateTemp1(float *temp1Reads, uint8_t iteration, uint8_t packetsCounter) {
  Packet sensorPacket;
  for (int i=0; i<NUM_4_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.fourByte[i] = temp1Reads[i*2];
  }
  setPacketFields(&sensorPacket, TEMP1_FN, iteration, packetsCounter);
  return sensorPacket;
}

Packet generateTemp1Compressed(float *temp1Reads, uint8_t iteration, uint8_t packetsCounter) {
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = compressIMUTemp(temp1Reads[i]);
  }
  setPacketFields(&sensorPacket, TEMP1_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateTemp2(uint16_t *temp2Reads, uint8_t iteration, uint8_t packetsCounter) {
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = temp2Reads[i];
  }
  setPacketFields(&sensorPacket, TEMP2_FN, iteration, packetsCounter);
  return sensorPacket;
}

Packet generateLightToFrequency(uint16_t *lightToFrequencyReads, uint8_t iteration, uint8_t packetsCounter) {
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = lightToFrequencyReads[i];
  }
  setPacketFields(&sensorPacket, LIGHT_TO_FREQUENCY_FN, iteration, packetsCounter);
  return sensorPacket;
}  

Packet generateGamma(uint16_t *gammaReads, uint8_t iteration, uint8_t packetsCounter) {
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = gammaReads[i*2];
  }
  setPacketFields(&sensorPacket, GAMMA_FN, iteration, packetsCounter);
  return sensorPacket;
}

Packet generateGammaComp(uint16_t *gammaReads, uint8_t iteration, uint8_t packetsCounter) {
  Packet sensorPacket;
  for (int i=0; i<NUM_1_BYTE_READINGS; i++) {
    uint16_t val = gammaReads[i];
    if (val > 255) {
      val = 255;
    }      
    sensorPacket.ArrayType.oneByte[i] = (uint8_t)val;
  }
  setPacketFields(&sensorPacket, GAMMA_FN_COMP, iteration, packetsCounter);
  return sensorPacket;
}

Packet generateCurrentSenseResistor(uint16_t *currentSenseReads, uint8_t iteration, uint8_t packetsCounter) {
  Packet sensorPacket;
  for (int i=0; i<NUM_2_BYTE_READINGS; i++) {
    sensorPacket.ArrayType.twoByte[i] = currentSenseReads[i];
  }
  setPacketFields(&sensorPacket, CURRENT_SENSE_RESISTOR_FN, iteration, packetsCounter);
  return sensorPacket;
}  
