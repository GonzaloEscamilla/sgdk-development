#pragma once

#include "genesis.h"
#include <resources.h>

#define EASE_DIRECTION_UP 0
#define EASE_DIRECTION_DOWN 1
#define EASE_DIRECTION_LEFT 2
#define EASE_DIRECTION_RIGHT 3

typedef enum EaseType_
{
    EaseLinear,
    EaseInExpo,
    EaseOutExpo
} EaseType;

/* typedef struct Tweener_
{
    Object;
    Sprite* currentSprite;
    EaseType easeType;
    Vect2D_s16 destination;
    Vect2D_f16 currentPosition;
    fix16 speed;
} Tweener; */

typedef struct Tweener_
{
    Object;
    Sprite* currentSprite;
    Vect2D_f16 currentPosition;
    fix16 remainingDistance;
    fix16 speed;
    u8 direction;
    EaseType easeType;
    void* Completed;
} Tweener;

extern Pool* tweenersPool;

void TWEEN_Init();
void TWEEN_Update();
void TWEEN_Kill(Tweener* tweener);
void TWEEN_SetCompletedCallback(Tweener* tweener, void (*callback)(Tweener*));
void TWEEN_SetEase(Tweener* tweener, EaseType easeType);
Tweener* TWEEN_Move1D(Sprite* spriteToMove, Vect2D_s16 from, u8 direction, u16 displacementAmount, fix16 speed);