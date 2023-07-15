#include "../inc/coinsController.h"

#define COIN_HALF_WIDTH 8
#define COIN_HALF_HIGHT 8

Pool* coinsPool;

void InitCallback(Object* obj);
void UpdateCallback(Object* obj);
void EndCallback(Object* obj);
void Cleanup();

void COINS_Init()
{
    coinsPool = OBJ_createObjectPool(16, sizeof(Coin));
    return coinsPool;
}

void COINS_Spawn(u16 x, u16 y)
{
    Coin* coin = (Coin*)OBJ_create(coinsPool);

    // Set the methods for the entity object
    OBJ_setInitMethod((Object*)coin, InitCallback);
    OBJ_setUpdateMethod((Object*)coin, UpdateCallback);
    OBJ_setEndMethod((Object*)coin, EndCallback);
    
    coin->position.x = x;
    coin->position.y = y;
    coin->init(coin);
}

void COINS_Update()
{
    kprintf("COINS_Update: ""%i",(POOL_getNumAllocated(coinsPool)));
    OBJ_updateAll(coinsPool);
}

void COINS_Collect(Coin* coin)
{
    coin->end(coin);
    OBJ_release(coinsPool, coin, TRUE);
}

void InitCallback(Object* obj)
{
    Coin* coin = (Coin*)obj;
    kprintf("Init Coin: ""%i", fix16ToInt(coin->position.x)); 
    coin->sprite = SPR_addSprite(&spr_coin, fix16ToInt(coin->position.x) - COIN_HALF_WIDTH, fix16ToInt(coin->position.y) - COIN_HALF_HIGHT, TILE_ATTR(PAL2, 1, FALSE, FALSE));
}

void UpdateCallback(Object* obj)
{
    Coin* coin = (Coin*)obj;
    // Does nothing yet
    kprintf("Update Callback"); 

    SPR_setPosition(coin->sprite, fix16ToInt(coin->position.x), fix16ToInt(coin->position.y));
}

void EndCallback(Object* obj)
{
    Coin* coin = (Coin*)obj;
    SPR_releaseSprite(coin->sprite);
}

void Cleanup()
{
    // Release all objects in the pool
    while (POOL_getNumAllocated(coinsPool) > 0)
    {
        Object* obj = (Object*)POOL_getFirst(coinsPool);
        OBJ_release(coinsPool, obj, TRUE);
    }

    // Destroy the pool
    POOL_destroy(coinsPool);
}
