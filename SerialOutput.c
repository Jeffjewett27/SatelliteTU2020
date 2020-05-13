/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "simpletools.h"
#include "fdserial.h"
#include "SerialOutput.h"
#include "EEPROM.h"

const int BYTES_PER_PACKET = 35;
const char ACK[] = {0xAA, 0x05, 0x00};
const int NAK = 0xAA05FF;
const int BUSY_PIN = 16;

const int SERIAL_RX = 17;
const int SERIAL_TX = 18;

fdserial *sr;
queue *packetQueue;
int *thread;

//initializes pins
void initSerial() {
  sr = fdserial_open(SERIAL_RX, SERIAL_TX, 0, 38400);
}  

//starts the serial output thread
void serialOutputThread(queue *pQueue) {
  packetQueue = pQueue;
  initSerial();
  thread = cog_run(serialOutputLoop, 128);
}  

//forever waits for busy to drop then sends first element of queue
void serialOutputLoop() {
  while(1) {
    while(isBusy()) {
      pause(100);
    }    
    if (isQueueEmpty(packetQueue)) {
      continue;
    }    
    Packet packet = peekQueue(packetQueue);
    outputPacket((char*)&packet);
    if (isACK()) {
      uint8_t packetCount = packet.packetsCounter;
      setPacketCount(packetCount);
      dequeue(packetQueue);
    }
  }  
}  

//outputs a packet
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

//determines if signal is ACK
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
  int val = 1; //note: safer comparison
  for (int i = 0; i < numACKBytes; ++i) {
    if (ACK[i] != response[i]) {
      val = 0;
    }      
  }    
  return val;
}  

//checks the busy pin
int isBusy() {
  return input(BUSY_PIN);
}  