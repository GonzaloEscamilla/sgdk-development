#pragma once

#include <genesis.h>
#include <resources.h>

typedef struct Coin_
{
    Object;
    Sprite* sprite;
    Vect2D_f16 position;
} Coin;

extern Pool* coinsPool;

void COINS_Init();
void COINS_Update();
void COINS_Spawn(s16 x, s16 y);
void COINS_Collect(Coin* coin);