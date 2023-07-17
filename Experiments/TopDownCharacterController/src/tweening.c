#include "../inc/tweening.h"

#define POOL_SIZE 8

void TweenInitCallback(Object* obj);
void TweenEndCallback(Object* obj);

void SetEaseTypeBasedUpdateCallback(Tweener* tweener);
void TweenLinearUpdate(Object* obj);
void TweenInExpoUpdate(Object* obj);

bool IsOnPoint(Tweener* tweener, Vect2D_s16 distance);


Pool* tweenersPool;

void TWEEN_Init()
{
    tweenersPool = POOL_create(POOL_SIZE, sizeof(Tweener));
}

void TWEEN_Update()
{
    OBJ_updateAll(tweenersPool);
}

void TWEEN_SetEase(Tweener* tweener, EaseType easeType)
{
    tweener->easeType = easeType;
    SetEaseTypeBasedUpdateCallback(tweener);
}

Tweener* TWEEN_MoveFromTo(Sprite* spriteToMove, Vect2D_s32 from, Vect2D_s32 to, fix16 speed)
{
    Tweener* tweener = (Tweener*)POOL_allocate(tweenersPool);
    
    tweener->currentSprite = spriteToMove;
    tweener->destination.x = to.x;
    tweener->destination.y = to.y;
    tweener->currentPosition.x = intToFix16(from.x); 
    tweener->currentPosition.y = intToFix16(from.y); 
    tweener->speed = speed;
    tweener->easeType = Linear;

    OBJ_setInitMethod((Object*)tweener, TweenInitCallback);
    OBJ_setUpdateMethod((Object*)tweener, TweenLinearUpdate);
    OBJ_setEndMethod((Object*)tweener, TweenEndCallback);

    tweener->init(tweener);

    return tweener;
}

void TweenInitCallback(Object* obj)
{
    
}

void SetEaseTypeBasedUpdateCallback(Tweener* tweener)
{
    switch (tweener->easeType)
    {
        case Linear:
            OBJ_setUpdateMethod((Object*)tweener, TweenLinearUpdate);
            break;
        case EaseInExpo:
            OBJ_setUpdateMethod((Object*)tweener, TweenInExpoUpdate);
            break;
        case EaseOutExpo:
            break;
        default:
            break;
    }
}



void TweenInExpoUpdate(Object* obj)
{
    Tweener* tweener = (Tweener*)obj;

    if (tweener->currentSprite == NULL)
            return;

    Vect2D_s16 distance;

    distance.x = tweener->destination.x - fix16ToInt(tweener->currentPosition.x);
    distance.y = tweener->destination.y - fix16ToInt(tweener->currentPosition.y);

    if (distance.x == 0 && distance.y == 0)
        return;

    if (IsOnPoint(tweener, distance))
        return;

    kprintf("Current Speed: ""%i", fix16ToInt(tweener->speed)); 
    kprintf("Current Speed: ""%10i", fix16ToInt(tweener->speed)); 
    
    fix16 xMovement = 0;
    if (distance.x > 0)
        xMovement = tweener->speed;
    else if(distance.x < 0)
        xMovement = -tweener->speed;
    else
        xMovement = 0; 
    
    fix16 yMovement = 0;
    if (distance.y > 0)
        yMovement = tweener->speed;
    else if(distance.y < 0)
        yMovement = -tweener->speed;
    else
        yMovement = 0; 
    
    if (tweener-> speed > FIX16(0))
    {
        tweener->speed += FIX16(0.25);
    }
    if (tweener-> speed < FIX16(0))
    {
        tweener->speed -= FIX16(0.25);
    }
    

    tweener->currentPosition.x += xMovement;
    tweener->currentPosition.y += yMovement;

    SPR_setPosition(tweener->currentSprite, fix16ToInt(tweener->currentPosition.x), fix16ToInt(tweener->currentPosition.y));
}

void TweenLinearUpdate(Object* obj)
{
    Tweener* tweener = (Tweener*)obj;

    if (tweener->currentSprite == NULL)
            return;

    Vect2D_s16 distance;

    distance.x = tweener->destination.x - tweener->currentPosition.x;
    distance.y = tweener->destination.y - tweener->currentPosition.y;

    if (distance.x == 0 && distance.y == 0)
        return;

    kprintf("Current X: ""%i", distance.x); 
    
    s16 xMovement = 0;
    if (distance.x > 0)
        xMovement = tweener->speed;
    else if(distance.x < 0)
        xMovement = -tweener->speed;
    else
        xMovement = 0; 
    
    s16 yMovement = 0;
    if (distance.y > 0)
        yMovement = tweener->speed;
    else if(distance.y < 0)
        yMovement = -tweener->speed;
    else
        yMovement = 0; 

    tweener->currentPosition.x += xMovement;
    tweener->currentPosition.y += yMovement;

    SPR_setPosition(tweener->currentSprite, tweener->currentPosition.x, tweener->currentPosition.y);
}

void TweenEndCallback(Object* obj)
{
    Tweener* tweener = (Tweener*)obj;
    SPR_releaseSprite(tweener->currentSprite);
}

void TweenCleanup()
{
    // Release all objects in the pool
    while (POOL_getNumAllocated(tweenersPool) > 0)
    {
        Object* obj = (Object*)POOL_getFirst(tweenersPool);
        OBJ_release(tweenersPool, obj, TRUE);
    }

    // Destroy the pool
    POOL_destroy(tweenersPool);
}

bool IsOnPoint(Tweener* tweener, Vect2D_s16 distance)
{
    kprintf("Distance ""%i", distance.x); 

    bool isSpeedPositive = tweener->speed > FIX16(0)? TRUE: FALSE;
    bool isDistancePositive = distance.x > 0 ? TRUE: FALSE;

    kprintf("ISSpeedPositive: ""%i", isSpeedPositive); 
    kprintf("IsDistancePositive: ""%i", isDistancePositive); 


     if (isSpeedPositive && isDistancePositive)
    {
        if (fix16ToInt(tweener->speed) > distance.x)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    } 
    return FALSE;
    
}
