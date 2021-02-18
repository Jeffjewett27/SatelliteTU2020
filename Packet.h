#ifndef PACKET_H
#define PACKET_H

#include "simpletools.h"

//A packet which will be transmitted
struct Packet {
  uint8_t fnCode;
  union {
    uint8_t oneByte[32];
    uint16_t twoByte[16];
    float fourByte[8];
  } ArrayType;
  uint8_t iteration;
  uint8_t packetsCounter;
};

typedef struct Packet Packet;

#endif