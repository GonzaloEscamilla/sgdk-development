#include <genesis.h>

#ifndef _QUEUE_VECT2D_H_
#define _QUEUE_VECT2D_H_

Vect2D_u32 QUEUE_getRange();

int QUEUE_getRear();

Vect2D_u32 QUEUE_getElementAt(int index);

Vect2D_u32 QUEUE_peek();

Vect2D_u32 QUEUE_peekRear();

bool QUEUE_isEmpty();

bool QUEUE_isFull();

u32 QUEUE_size();

void QUEUE_insert(Vect2D_u32 data);

Vect2D_u32 QUEUE_removeData();

Vect2D_u32 QUEUE_deQueue();


#endif
