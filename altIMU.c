#include "DataConversion.h"
#include "altIMU.h"

Packet getIMUPacketX(Vector3 *readings, uint8_t code, uint8_t iteration, uint8_t pc, int offset) {
  Packet packet;
  packet.fnCode = code;
  packet.iteration = iteration;
  packet.packetsCounter = pc;
  for (int i=0; i<16; i++) {
    packet.ArrayType.twoByte[i] = reduceFloat16bit(readings[i + 16*offset].x, 1, 5);
  }
  return packet;
}  

Packet getIMUPacketY(Vector3 *readings, uint8_t code, uint8_t iteration, uint8_t pc, int offset) {
  Packet packet;
  packet.fnCode = code;
  packet.iteration = iteration;
  packet.packetsCounter = pc;
  for (int i=0; i<16; i++) {
    packet.ArrayType.twoByte[i] = reduceFloat16bit(readings[i + 16*offset].y, 1, 5);
  }
  return packet;
}  

Packet getIMUPacketZ(Vector3 *readings, uint8_t code, uint8_t iteration, uint8_t pc, int offset) {
  Packet packet;
  packet.fnCode = code;
  packet.iteration = iteration;
  packet.packetsCounter = pc;
  for (int i=0; i<16; i++) {
    packet.ArrayType.twoByte[i] = reduceFloat16bit(readings[i + 16*offset].z, 1, 5);
  }
  return packet;
}