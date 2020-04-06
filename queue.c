/*
 * Author: Caleb
 * Modification Date: April 5, 2020
 */

#include "simpletools.h"
#include "queue.h"

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
      
Packet peekQueue(queue *q){
  return(q->front->value);
}
  
//int is a placeholder for testing
void enqueue(queue *q, Packet val){
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
    
Packet dequeue(queue *q){
 node *finger = q->front;
 Packet value = q->front->value;
 q->front = q->front->next;
 q->size--;
 free(finger);
 if(q->size == 0) initializeQueue(q);
 return value;
}

/*
int main(){
  queue *q;
  q = malloc(sizeof(queue));
  initializeQueue(q);    
} 
*/  
