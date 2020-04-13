#include "simpletools.h" // Include simple tools
#include "fdserial.h"
#include "SerialOutput.h"

const int BYTES_PER_PACKET = 35;
const int ACK = 0xAA0500;
const int NAK = 0xAA05FF;
const int BUSY_PIN = 10;

fdserial *sr;
queue *packetQueue;
int *thread;

void serialOutputThread(queue *pQueue) {
  packetQueue = pQueue;
  initSerial();
  thread = cog_run(serialOutputLoop, 128);
}  

void serialOutputLoop() {
  while(1) {
    while(isBusy()) {
      pause(100);
    }    
    if (isQueueEmpty(packetQueue)) {
      return;
    }    
    Packet packet = peekQueue(packetQueue);
    outputPacket((char*)&packet);
    if (isACK()) {
      dequeue(packetQueue);
    }
  }  
}  

void outputPacket(char* packet) {
  fdserial_rxFlush(sr);
  fdserial_txFlush(sr);

  //header bytes
  fdserial_txChar(sr, 0x50);
  fdserial_txChar(sr, 0x50);
  fdserial_txChar(sr, 0x50);

  for(int i=0; i<BYTES_PER_PACKET; i++) {
    fdserial_txChar(sr, packet[i]);
  }
}

int isACK() {
  int numACKBytes = 3;
  char response[numACKBytes];
  for (int i = 0; i < numACKBytes; ++i) {
    int safetyEscape = 0;
    while(fdserial_rxCount(sr)==0) {
      if (safetyEscape++ > 1500) {
        //ensure we don't get caught in an infinite loop. 1500 iterations * 10ms/iter = 15s. 
        //It should not take anywhere near that long or something is seriously wrong
        return 0;
      }       
      pause(10);
    }
    response[i] = fdserial_rxChar(sr);
  }
  int val = (*((uint32_t*)response)) >> 8;
  return val == ACK;
}  

int isBusy() {
  return input(BUSY_PIN);
}  