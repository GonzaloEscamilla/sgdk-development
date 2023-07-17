#pragma once

#include "genesis.h"
#include <resources.h>

typedef enum EaseType_
{
    Linear,
    EaseInExpo,
    EaseOutExpo
} EaseType;

typedef struct Tweener_
{
    Object;
    Sprite* currentSprite;
    EaseType easeType;
    Vect2D_s16 destination;
    Vect2D_f16 currentPosition;
    fix16 speed;
} Tweener;

extern Pool* tweenersPool;

void TWEEN_Init();
void TWEEN_Update();
void TWEEN_SetEase(Tweener* tweener, EaseType easeType);
Tweener* TWEEN_MoveFromTo(Sprite* spriteToMove, Vect2D_s32 from, Vect2D_s32 to, fix16 speed);