#include "queue.h"

void serialOutputThread(queue *pQueue);
void serialOutputLoop();
void initSerial();
void outputPacket(char* packet);
int isACK();
int isBusy();