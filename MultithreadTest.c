#include "simpletools.h"
#include "queue.h"
#include "MultithreadTest.h"

int *thread;
queue *packetQueue;

void startTest() {
  queue *createdQueue;
  createdQueue = malloc(sizeof(queue));
  initializeQueue(createdQueue);
  startThread(createdQueue);
  for (int i = 0; i < 24; i++) {
    if(isQueueEmpty(createdQueue)) {
      print("%d: queue is empty\n", i);
      pause(1000);
      continue;
    }      
    Packet packet = dequeue(createdQueue);
    print("packet%d:", i);
    for (int j = 0; j < 35; j++) {
      print("%x ", packet.ArrayType.oneByte[j]);
    }      
    print("\n");
    pause(1000);
  }  
}

void startThread(queue *testQueue) {
  packetQueue = testQueue;
  thread = cog_run(testLoop, 128);
}  

void testLoop() {
  pause(500);
  for (int i = 0; i < 20; ++i) {
    Packet packet;
    for (int j = 0; j < 32; ++j) {
      packet.ArrayType.oneByte[j] = i + j;
    }    
    enqueue(packetQueue, packet);
    pause(1000);
  }  
  cog_end(thread);
}  