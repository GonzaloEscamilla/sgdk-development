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


const BoxCollider characterBox = {
    .min = { PLAYER_COLBOX_LEFT, PLAYER_COLBOX_TOP },
    .max = { PLAYER_COLBOX_RIGHT, PLAYER_COLBOX_BOTTOM }
};
 
Player player;

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

    
    s16 topLeftCollisionTile = characterBounds.min.x >> 4;
    s16 topRightCollisionTile = characterBounds.max.x >> 4;
    s16 bottomLeftCollisionTile = characterBounds.min.y >> 4;
    s16 bottomRightCollisionTile = characterBounds.max.y >> 4;

    u16 topLeftCollisionTileIndex = topLeftCollisionTile + LENGHT_OF_LEVELCOL_ARRAY;
    u8 topLeftTileType = ROOM_A_COLLISIONS[topLeftCollisionTileIndex];

/*     kprintf("%i", characterBounds.min.x >> 4);
    kprintf("%i", characterBounds.min.y >> 4);
    kprintf("%i", characterBounds.max.x >> 4);
    kprintf("%i", characterBounds.max.y >> 4);  */

    kprintf("%i", topLeftTileType); 

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