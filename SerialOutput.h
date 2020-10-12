/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

#include "queue.h"

//starts the serial output thread
void serialOutputThread(queue *pQueue);

//forever waits for busy to drop then sends first element of queue
void serialOutputLoop();

//initializes pins
void initSerial();

//outputs a packet
void outputPacket(Packet *packet);

//waits and returns whether response is ack
int isACK();

//is busy
int isBusy();