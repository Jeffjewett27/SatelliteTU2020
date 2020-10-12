#include "simpletools.h"
#include "Packet.h"
#include "Vector3.h"

Packet getIMUPacketX(Vector3 *readings, uint8_t code, uint8_t iteration, uint8_t pc, int offset);
Packet getIMUPacketY(Vector3 *readings, uint8_t code, uint8_t iteration, uint8_t pc, int offset);
Packet getIMUPacketZ(Vector3 *readings, uint8_t code, uint8_t iteration, uint8_t pc, int offset);
