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
  q->front = 0;
  q->back = 0;
}
  
int isEmpty(queue *q){
  return (getSize(q) == 0);
}
  
void clear(queue *q){
  while(q->size != 0){
    dequeue(q);
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
  finger->next = 0;
  if(isEmpty(q)){
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
 if(q->size == 0) initialize(q);
 return value;
}

int main(){
  queue *q;
  q = malloc(sizeof(queue));
  initialize(q);
  /*
  printf("%d\n",isEmpty(q));
  enqueue(q,9);
  enqueue(q,90);
  enqueue(q,55);
  enqueue(q,12);
  enqueue(q,45);
  enqueue(q,22);
  enqueue(q,6);
  
  
  clear(q);
  printf("Peek: %d",peek(q));
  
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
  
  print("done");   */    
}  
