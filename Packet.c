#include "Packet.h"

void printPacket(Packet packet) {
  return;
  print("code: %02x, iter: %02x, pc: %02x, data: ", packet.fnCode, packet.iteration, packet.packetsCounter);
  //for (int i = 0; i < 32; i++) {
    //print("%02x ", packet.ArrayType.oneByte[i]);
  //}
  print("\n");
}  

void printPacketRaw(char* packet) {
  for (int i = 0; i < 35; i++) {
    print("%02x ", packet[i]);
  }
  print("\n");
}  

void makeTestPacket(Packet* packet) {
  packet->fnCode = 0xe3;
  packet->iteration = 0xe4;
  packet->packetsCounter = 0xe5;
  for (int i = 0; i < 32; i++) {
    packet->ArrayType.oneByte[i] = i;
  }    
}  