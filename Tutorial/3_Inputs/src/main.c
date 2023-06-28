#include "genesis.h"
#include <resources.h>

#define ANIMATION_SWITCH_SPEED 60

Sprite* player;
Sprite* player2;

int player_x = 100;
int player_y = 50;

int player2_x = 120;
int player2_y = 75;

#define ANIM_IDLE_FRONT 0
#define ANIM_IDLE_BACK 1
#define ANIM_IDLE_RIGHT 2
#define ANIM_IDLE_LEFT 3
#define ANIM_WALK_FRONT 4
#define ANIM_WALK_BACK 5
#define ANIM_WALK_RIGHT 6
#define ANIM_WALK_LEFT 7


static void handleInput()
{
    u16 joy1Value = JOY_readJoypad(JOY_1);

    if (joy1Value & BUTTON_LEFT)
    {
        player_x -= 1;
        SPR_setAnim(player, ANIM_WALK_LEFT);
    }
    else if (joy1Value & BUTTON_RIGHT)
    {
        player_x += 1;
        SPR_setAnim(player, ANIM_WALK_RIGHT);
    }
    
    if (joy1Value & BUTTON_UP)
    {
        player_y -= 1;
        SPR_setAnim(player, ANIM_WALK_BACK);

    }
    else if (joy1Value & BUTTON_DOWN)
    {
        player_y += 1;
        SPR_setAnim(player, ANIM_WALK_FRONT);
    }

    if(!(joy1Value & BUTTON_LEFT) && !(joy1Value & BUTTON_RIGHT) && !(joy1Value & BUTTON_DOWN) && !(joy1Value & BUTTON_UP))
    {
        SPR_setAnimAndFrame(player, ANIM_IDLE_FRONT, 0);
        player->timer = 0;
    }

}

static void joyEvent(u16 joy, u16 changed, u16 state)
{

}

int main(bool hardReset)
{
    SPR_init();
    
    PAL_setPalette(PAL1, spr_player.palette->data, DMA);
    player = SPR_addSprite(&spr_player, player_x, player_y, TILE_ATTR(PAL1, 0, FALSE, FALSE));

    PAL_setPalette(PAL2, spr_player2.palette->data, DMA);
    player2 = SPR_addSprite(&spr_player2, player_x, player_y, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    JOY_setEventHandler(joyEvent);

    while (TRUE)
    {
        handleInput();
        SPR_setPosition(player, player_x, player_y);
        SPR_setPosition(player2, player2_x, player2_y);

        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}
