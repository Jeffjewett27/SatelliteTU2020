/*
 * Author: Caleb
 * Modification Date: April 5, 2020
 */

#include "Packet.h"

struct node{
  Packet value;
  struct node *next;
};
typedef struct node node;

struct queue{
  int size;
  node *front;
  node *back;
};
typedef struct queue queue;

// Prototypes
void initializeQueue(queue *q);
int isQueueEmpty(queue *q);
void clearQueue(queue *q);
int getQueueSize(queue *q);
Packet peekQueue(queue *q);
void enqueue(queue *q, Packet val);
Packet dequeue(queue *q);


