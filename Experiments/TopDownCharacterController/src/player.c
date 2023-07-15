#include "../inc/player.h"
#include "../inc/inputs.h"

#define ANIMATION_SWITCH_SPEED 60
#define ANIM_IDLE_FRONT 0
#define ANIM_IDLE_BACK 1
#define ANIM_IDLE_RIGHT 2
#define ANIM_IDLE_LEFT 3
#define ANIM_WALK_FRONT 4
#define ANIM_WALK_BACK 5
#define ANIM_WALK_RIGHT 6
#define ANIM_WALK_LEFT 7

#define CHECK_INTERACTABLES_RATE 6
#define DISTANCE_TO_COLLECTABLE_THERESHOLD 16

const BoxCollider characterBox = {
    .min = { PLAYER_COLBOX_LEFT, PLAYER_COLBOX_TOP },
    .max = { PLAYER_COLBOX_RIGHT, PLAYER_COLBOX_BOTTOM }
};

void collectCoin();
void checkInteractables();
Vect2D_s32 getPlayerCenter();

Player player;
u8 checkCounter = 0;

void PLAYER_Init()
{
    player.velocity.x = FIX32(0.0f);
    player.velocity.y = FIX32(0.0f);
    player.position.x = FIX32(80);
    player.position.y = FIX32(80);

    player.speed = FIX32(1.5);
    player.diagonalSpeed = FIX32(1.060);
    player.sprite = SPR_addSprite(&spr_player, player.position.x, player.position.y, TILE_ATTR(PAL1, 0, FALSE, FALSE)); 
    PAL_setPalette(PAL1, spr_player.palette->data, DMA);
}

void PLAYER_Update()
{
    switch (player.currentDirection)
    {
        case Left:
            player.position.x -= player.speed;
            break;
        case Right:
            player.position.x += player.speed;
            break;
        case Back:
            player.position.y -= player.speed;
            break;
        case Front:
            player.position.y += player.speed;
            break;
        case FrontLeft:
            player.position.x -= player.diagonalSpeed;
            player.position.y += player.diagonalSpeed;
            break;
        case FrontRight:
            player.position.x += player.diagonalSpeed;
            player.position.y += player.diagonalSpeed;
            break;
        case BackLeft:
            player.position.x -= player.diagonalSpeed;
            player.position.y -= player.diagonalSpeed;
            break;
        case BackRight:
            player.position.x += player.diagonalSpeed;
            player.position.y -= player.diagonalSpeed;
            break;
        default:
            break;
    }

    PLAYER_checkCollisions();
} 

char buffer[10];

void PLAYER_checkCollisions()
{
    BoxCollider characterBounds = COL_newBoxCollider(
			fix32ToInt(player.position.x) + characterBox.min.x,
			fix32ToInt(player.position.x) + characterBox.max.x,
			fix32ToInt(player.position.y) + characterBox.min.y,
			fix32ToInt(player.position.y) + characterBox.max.y
		);

    s16 leftCollisionXCoord = characterBounds.min.x >> 4;
    s16 rightCollisionXCoord = characterBounds.max.x >> 4;
    s16 topCollisionYCoord = characterBounds.min.y >> 4;
    s16 bottomCollisionYCoord = characterBounds.max.y >> 4;

    // Note: Doing a bit shift while be better. Or a matrix, for the bit shif you need a 8 multiple collider level array
    s16 topLeftCollisionTileCoord = leftCollisionXCoord + (topCollisionYCoord * LENGHT_OF_LEVELCOL_ARRAY); 
    s16 topRightCollisionTileCoord = rightCollisionXCoord + (topCollisionYCoord * LENGHT_OF_LEVELCOL_ARRAY); 
    s16 bottomLeftCollisionTileCoord = leftCollisionXCoord + (bottomCollisionYCoord * LENGHT_OF_LEVELCOL_ARRAY); 
    s16 bottomRightCollisionTileCoord = rightCollisionXCoord + (bottomCollisionYCoord * LENGHT_OF_LEVELCOL_ARRAY); 

/*     u8 topLeftTileType =  ROOM_B_COLLISIONS[topLeftCollisionTileCoord];
    u8 topRightTileType = ROOM_B_COLLISIONS[topRightCollisionTileCoord];
    u8 bottomLeftTileType = ROOM_B_COLLISIONS[bottomLeftCollisionTileCoord];
    u8 bottomRightTileType = ROOM_B_COLLISIONS[bottomRightCollisionTileCoord]; */

    
    u8 topLeftTileType =  currentLevel->collisionMap[topLeftCollisionTileCoord];
    u8 topRightTileType = currentLevel->collisionMap[topRightCollisionTileCoord];
    u8 bottomLeftTileType = currentLevel->collisionMap[bottomLeftCollisionTileCoord];
    u8 bottomRightTileType = currentLevel->collisionMap[bottomRightCollisionTileCoord];
    
    switch (player.currentDirection)
    {
        case Left:
            if (topLeftTileType == SOLID_TILE || bottomLeftTileType == SOLID_TILE)
            {
                player.position.x =  intToFix32((leftCollisionXCoord << 4) + LEVEL_TILE_SIZE - PLAYER_COLBOX_LEFT);
            }
            
            break;
        case Right:
            if (topRightTileType == SOLID_TILE || bottomRightTileType == SOLID_TILE)
            {
                player.position.x =  intToFix32((rightCollisionXCoord << 4) - PLAYER_COLBOX_RIGHT);
                player.position.x -= PLAYER_DEPENETRATION;
            }
            break;
        case Back:
            if (topLeftTileType == SOLID_TILE || topRightTileType == SOLID_TILE)
            {
                player.position.y =  intToFix32((topCollisionYCoord << 4) + LEVEL_TILE_SIZE - PLAYER_COLBOX_TOP);
            }
            break;
        case Front:
            if (bottomLeftTileType == SOLID_TILE || bottomRightTileType == SOLID_TILE)
            {
                player.position.y =  intToFix32((bottomCollisionYCoord << 4) - PLAYER_COLBOX_BOTTOM);
                player.position.y -= PLAYER_DEPENETRATION;
            }
            break;
        case BackLeft:
            if (bottomLeftTileType == SOLID_TILE || topRightTileType == SOLID_TILE)
            {
                if (bottomLeftTileType == SOLID_TILE)
                {
                    player.position.x =  intToFix32((leftCollisionXCoord << 4) + LEVEL_TILE_SIZE - PLAYER_COLBOX_LEFT);
                }
                if (topRightTileType == SOLID_TILE)
                {
                    player.position.y =  intToFix32((topCollisionYCoord << 4) + LEVEL_TILE_SIZE - PLAYER_COLBOX_TOP);
                }
            }
            else if (topLeftTileType == SOLID_TILE)
            {
                if (characterBounds.min.x <= ((leftCollisionXCoord << 4) + PLAYER_COLBOX_LEFT +2))
                {
                    player.position.y =  intToFix32((topCollisionYCoord << 4) + LEVEL_TILE_SIZE - PLAYER_COLBOX_TOP);
                }
                else if (characterBounds.min.x > ((leftCollisionXCoord << 4) + PLAYER_COLBOX_LEFT+2 ))
                {
                    player.position.x =  intToFix32((leftCollisionXCoord << 4) + LEVEL_TILE_SIZE - PLAYER_COLBOX_LEFT);
                }
            }   
            break;
        case BackRight:
            if (topLeftTileType == SOLID_TILE || bottomRightTileType == SOLID_TILE)
            {
                if (bottomRightTileType == SOLID_TILE)
                {
                    player.position.x =  intToFix32((rightCollisionXCoord << 4) - PLAYER_COLBOX_RIGHT);
                    player.position.x -= PLAYER_DEPENETRATION;
                }
                if (topLeftTileType == SOLID_TILE)
                {
                    player.position.y = intToFix32((topCollisionYCoord << 4) + LEVEL_TILE_SIZE - PLAYER_COLBOX_TOP);
                }
            }
            else if (topRightTileType == SOLID_TILE)
            {
                if (characterBounds.max.x <= ((rightCollisionXCoord << 4)))
                {
                    player.position.x =  intToFix32((rightCollisionXCoord << 4) - PLAYER_COLBOX_RIGHT);
                    player.position.x -= PLAYER_DEPENETRATION;

                }
                else if (characterBounds.max.x > ((rightCollisionXCoord << 4)))
                {
                    player.position.y = intToFix32((topCollisionYCoord << 4) + LEVEL_TILE_SIZE - PLAYER_COLBOX_TOP);
                }
            }  
            break;
        case FrontLeft:
            if (topLeftTileType == SOLID_TILE || bottomRightTileType == SOLID_TILE)
            {
                if (topLeftTileType == SOLID_TILE)
                {
                    player.position.x = intToFix32((leftCollisionXCoord << 4) + LEVEL_TILE_SIZE - PLAYER_COLBOX_LEFT);
                    player.position.x += PLAYER_DEPENETRATION;
                }
                if (bottomRightTileType == SOLID_TILE)
                {
                    player.position.y =  intToFix32((bottomCollisionYCoord << 4) - PLAYER_COLBOX_BOTTOM);
                    player.position.y -= PLAYER_DEPENETRATION;
                }
            }
            else if (bottomLeftTileType == SOLID_TILE)
            {
                if (characterBounds.min.x <= ((leftCollisionXCoord << 4) + PLAYER_COLBOX_LEFT +2))
                {
                    player.position.y =  intToFix32((bottomCollisionYCoord << 4) - PLAYER_COLBOX_BOTTOM);
                    player.position.y -= PLAYER_DEPENETRATION;
                }
                else if (characterBounds.min.x > ((leftCollisionXCoord << 4) + PLAYER_COLBOX_LEFT+2 ))
                {
                    player.position.x = intToFix32((leftCollisionXCoord << 4) + LEVEL_TILE_SIZE - PLAYER_COLBOX_LEFT);
                    player.position.x += PLAYER_DEPENETRATION;
                }
            }
            break;
        case FrontRight:
            if (topRightTileType == SOLID_TILE || bottomLeftTileType == SOLID_TILE)
                {
                    if (topRightTileType == SOLID_TILE)
                    {
                        player.position.x =  intToFix32((rightCollisionXCoord << 4) - PLAYER_COLBOX_RIGHT);
                        player.position.x -= PLAYER_DEPENETRATION;
                    }
                    if (bottomLeftTileType == SOLID_TILE)
                    {
                        player.position.y =  intToFix32((bottomCollisionYCoord << 4) - PLAYER_COLBOX_BOTTOM);
                        player.position.y -= PLAYER_DEPENETRATION;
                    }
                }
                else if (bottomRightTileType == SOLID_TILE)
                {
                    if (characterBounds.max.x <= ((rightCollisionXCoord << 4)))
                    {
                        player.position.x =  intToFix32((rightCollisionXCoord << 4) - PLAYER_COLBOX_RIGHT);
                        player.position.x -= PLAYER_DEPENETRATION;

                    }
                    else if (characterBounds.max.x > ((rightCollisionXCoord << 4)))
                    {
                        player.position.y =  intToFix32((bottomCollisionYCoord << 4) - PLAYER_COLBOX_BOTTOM);
                        player.position.y -= PLAYER_DEPENETRATION;
                    }
                }
                break;
        default:
            break;
    }
    
   checkInteractables();

    /*     
    kprintf("Top Left: ""%i", topLeftTileType); 
    kprintf("Top Right: ""%i", topRightTileType); 
    kprintf("Bottom Left: ""%i", bottomLeftTileType); 
    kprintf("Bottom Right: ""%i", bottomRightTileType);
    */

    VDP_drawTextBG(BG_A, buffer, 28, 10);
}

void PLAYER_InputChanged()
{
    u16 joy = input.joy;
	u16 changed = input.changed;
	u16 state = input.state;

    player.currentDirection = None;

    // We only read Joypad one
    if (joy != JOY_1)
        return;
    
if (state & BUTTON_RIGHT)
{
    if (state & BUTTON_UP)
        player.currentDirection = BackRight;
    else if (state & BUTTON_DOWN)
        player.currentDirection = FrontRight;
    else
        player.currentDirection = Right;
}
else if (state & BUTTON_LEFT)
{
    if (state & BUTTON_UP)
        player.currentDirection = BackLeft;
    else if (state & BUTTON_DOWN)
        player.currentDirection = FrontLeft;
    else
        player.currentDirection = Left;
}
else if (state & BUTTON_UP)
{
    player.currentDirection = Back;
}
else if (state & BUTTON_DOWN)
{
    player.currentDirection = Front;
}
else if ((changed & BUTTON_RIGHT) || (changed & BUTTON_LEFT) || (changed & BUTTON_UP) || (changed & BUTTON_DOWN))
{
    player.currentDirection = None;
}

    /*if(!(joy1Value & BUTTON_LEFT) && !(joy1Value & BUTTON_RIGHT) && !(joy1Value & BUTTON_DOWN) && !(joy1Value & BUTTON_UP))
    {
        //SPR_setAnimAndFrame(player, ANIM_IDLE_FRONT, 0);
        //player->timer = 0;
    } */
}

void checkInteractables()
{
    if ((checkCounter++ % CHECK_INTERACTABLES_RATE) != 0)
        return;
    
    kprintf("Check""%i", checkCounter); 

    Coin** coins = POOL_getFirst(coinsPool);
    u16 numCoins = POOL_getNumAllocated(coinsPool);
    u16 distanceX = 0;
    u16 distanceY = 0;
    
    Vect2D_s32 playerCenter = getPlayerCenter();

    kprintf("Num Coins ""%i", numCoins); 
    while (numCoins--)
    {
        Coin* coin = *coins++;
        distanceX = abs(fix16ToInt(coin->position.x) - playerCenter.x);
        distanceY = abs(fix16ToInt(coin->position.y) - playerCenter.y);
        if (distanceX <= DISTANCE_TO_COLLECTABLE_THERESHOLD && distanceY <= DISTANCE_TO_COLLECTABLE_THERESHOLD)
        {
            COINS_Collect(coin);
        }
    }
}

void collectCoin()
{

}

Vect2D_s32 getPlayerCenter()
{
    return newVector2D_s32(fix32ToInt(player.position.x) + PLAYER_HALF_WIDTH, fix32ToInt(player.position.y) + PLAYER_HALF_HEIGHT);
}