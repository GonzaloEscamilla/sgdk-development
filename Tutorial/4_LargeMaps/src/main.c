#include "genesis.h"
#include <resources.h>

#define ANIMATION_SWITCH_SPEED 60
#define ANIM_IDLE_FRONT 0
#define ANIM_IDLE_BACK 1
#define ANIM_IDLE_RIGHT 2
#define ANIM_IDLE_LEFT 3
#define ANIM_WALK_FRONT 4
#define ANIM_WALK_BACK 5
#define ANIM_WALK_RIGHT 6
#define ANIM_WALK_LEFT 7

Sprite* player;

u16 ind = TILE_USER_INDEX;
Map* level_1_map;

int player_x = 100;
int player_y = 50;

int mapHScrool = 0;

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
    VDP_loadTileSet(&our_tileset, ind, DMA);
    level_1_map = MAP_create(&our_level_map, BG_A, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind));
    PAL_setPalette(PAL0, our_palette.data, DMA);
    MAP_scrollTo(level_1_map, 0, 768);

    SPR_init();
    
    PAL_setPalette(PAL1, spr_player.palette->data, DMA);
    player = SPR_addSprite(&spr_player, player_x, player_y, TILE_ATTR(PAL1, 0, FALSE, FALSE));

    JOY_setEventHandler(joyEvent);

    while (TRUE)
    {
        MAP_scrollTo(level_1_map, mapHScrool++, 768);
        handleInput();
        SPR_setPosition(player, player_x, player_y);

        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}
