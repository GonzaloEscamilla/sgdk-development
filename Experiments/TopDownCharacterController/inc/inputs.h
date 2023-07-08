#ifndef _INPUTS_H_
#define _INPUTS_H_

#include <genesis.h>
#include <resources.h>

struct {
	u16 joy;
	u16 changed;
	u16 state;
}input;

void INPUT_Init();
void inGameJoyEvent(u16 joy, u16 changed, u16 state);

#endif