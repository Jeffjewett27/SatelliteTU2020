/*
 * Author: Caleb
 * Modification Date: April 1, 2020
 */

#include "simpletools.h"
#include "queue.h"

typedef struct node node;

typedef struct queue queue;

void initializeQueue(queue *q){
  q->size = 0;
  q->front = 0;
  q->back = 0;
}
  
int isQueueEmpty(queue *q){
  return (getQueueSize(q) == 0);
}
  
void clearQueue(queue *q){
  while(q->size != 0){
    dequeue(q);
  }
}

int getQueueSize(queue *q){
  return(q->size);
}  
      
int peekQueue(queue *q){
  return(q->front->value);
}
  
//int is a placeholder for testing
void enqueue(queue *q, int val){
  node *finger;
  finger = malloc(sizeof(node));
  finger->value = val;
  finger->next = 0;
  if(isQueueEmpty(q)){
    q->front = finger;
    q->back = q->front;
  }else{
    q->back->next = finger;
    q->back = finger;
  }
  q->size++;
}     
    
int dequeue(queue *q){
 node *finger = q->front;
 int value = q->front->value;
 q->front = q->front->next;
 q->size--;
 free(finger);
 if(q->size == 0) initializeQueue(q);
 return value;
}

int main(){
  queue *q;
  q = malloc(sizeof(queue));
  initializeQueue(q);
  /*
  printf("%d\n",isQueueEmpty(q));
  enqueue(q,9);
  enqueue(q,90);
  enqueue(q,55);
  enqueue(q,12);
  enqueue(q,45);
  enqueue(q,22);
  enqueue(q,6);
  
  
  clearQueue(q);
  printf("Peek: %d",peekQueue(q));
  
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peekQueue(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peekQueue(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peekQueue(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peekQueue(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peekQueue(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peekQueue(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peekQueue(q));  
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peekQueue(q));
  printf("dequeue: %d\n",dequeue(q));
  
  print("done");   */    
}  
