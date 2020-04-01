/*
 * Author: Caleb
 * Modification Date: April 1, 2020
 */

struct node{
  int value;
  struct node *next;
};

struct queue{
  int size;
  node *front;
  node *back;
};

// Prototypes
void initializeQueue(queue *q);
int isQueueEmpty(queue *q);
void clearQueue(queue *q);
int getQueueSize(queue *q);
int peekQueue(queue *q);
void enqueue(queue *q, int val);
int dequeue(queue *q);


