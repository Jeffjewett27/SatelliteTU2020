/*
 * Author: Caleb Eckerly
 * Modification Date: 4/5/20
 */

#include "Packet.h"

#ifndef QUEUE_NODE
#define QUEUE_NODE
struct node{
  Packet value;
  struct node *next;
};
typedef struct node node;
#endif

#ifndef QUEUE_STRUCT
#define QUEUE_STRUCT
struct queue{
  int size;
  node *front;
  node *back;
};
typedef struct queue queue;
#endif

// Prototypes
void initializeQueue(queue *q);
int isQueueEmpty(queue *q);
void clearQueue(queue *q);
int getQueueSize(queue *q);
Packet peekQueue(queue *q);
void enqueue(queue *q, Packet val);
Packet dequeue(queue *q);


