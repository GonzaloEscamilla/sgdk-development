#include "../inc/inputs.h"
#include "../inc/player.h"


void INPUT_Init()
{
    JOY_setEventHandler(inGameJoyEvent);
}

//In order to make this data more accessible from all scripts we write them into a struct from global.h
void inGameJoyEvent(u16 joy, u16 changed, u16 state) {
	input.joy = joy;
	input.changed = changed;
	input.state = state;

	PLAYER_InputChanged();
}