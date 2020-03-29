#include "simpletools.h"

struct node{
  int value;
  struct node *next;
};
typedef struct node node;

struct queue{
  int size;
  node *front;
  node *back;
};
typedef struct queue queue;

void initialize(queue *q){
  q->size = 0;
  q->front = -1;
  q->back = -1;
}
  
int isEmpty(queue *q){
  return (getSize(q) == 0);
}
  
//Seems to enter an infinit loop//
void clear(queue *q){
  while(!isEmpty(q)){
    dequeue(q);
    q->size--;
  }
}

int getSize(queue *q){
  return(q->size);
}  
      
int peek(queue *q){
  return(q->front->value);
}
  
//int is a placeholder for testing
void enqueue(queue *q, int val){
  node *finger;
  finger = malloc(sizeof(node));
  finger->value = val;
  finger->next = -1;
  if(isEmpty(q)){
    q->front = finger;
    q->back = q->front;
  }else if(getSize(q) == 1){
    q->back = finger;
    q->front->next = q->back;
  }else{
    q->back->next = finger;
    q->back = finger;
  }
  q->size++;
}     
    
//int is a placeholder for testing
//If there is nothing to dequeue it returns some int, attempting to put an 'else if return -1' statement results in returning -1 every time//
int dequeue(queue *q){
  node *finger;
  if(!isEmpty(q)){
    finger->value = q->front->value;
    q->front = q->front->next;
    q->size--;
    return(finger->value);
  } 
}  

int main(){
  queue *q;
  q = malloc(sizeof(queue));
  printf("%d\n",isEmpty(q));
  enqueue(q,9);
  enqueue(q,90);
  enqueue(q,55);
  enqueue(q,12);
  enqueue(q,45);
  enqueue(q,22);
  enqueue(q,6);
  
  
  //clear(q);
  //pause(2000);
  printf("Peek: %d",peek(q));
  /*
  //pause(1000);
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peek(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peek(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peek(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peek(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peek(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peek(q));
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peek(q));  
  printf("dequeue: %d\n",dequeue(q));
  printf("Peek: %d\n",peek(q));
  printf("dequeue: %d\n",dequeue(q));
  */
  print("done");       
}  