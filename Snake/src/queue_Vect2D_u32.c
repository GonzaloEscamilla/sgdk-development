#include <genesis.h>
#include "queue_Vect2D_u32.h"

#define MAX 255

Vect2D_u32 intArray[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;

Vect2D_u32 QUEUE_getRange()
{
   Vect2D_u32 range = { front, rear};
   return range;
}

int QUEUE_getRear()
{
   return rear;
}

Vect2D_u32 QUEUE_getElementAt(int index)
{
   return intArray[index];
}

Vect2D_u32 QUEUE_peek() {
   return intArray[front];
}

Vect2D_u32 QUEUE_peekRear() {
   return intArray[rear];
}

bool QUEUE_isEmpty() {
   return itemCount == 0;
}

bool QUEUE_isFull() {
   return itemCount == MAX;
}

u32 QUEUE_size() {
   return itemCount;
}  

void QUEUE_insert(Vect2D_u32 data) {

   if(!QUEUE_isFull()) {
	
      if(rear == MAX-1) {
         rear = -1;            
      }       

      intArray[++rear] = data;
      itemCount++;
   }
}

Vect2D_u32 QUEUE_removeData() 
{
   Vect2D_u32 data = intArray[front++];
   
   if(front == MAX) {
      front = 0;
   }
   
   itemCount--;

   KLog_U2("RData x: ", data.x, "RData y: ", data.y);

   return data;  
}

Vect2D_u32 QUEUE_deQueue() 
{
   return QUEUE_removeData();
}