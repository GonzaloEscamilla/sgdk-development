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

#define HOW_FAR_TO_LEFT_BEFORE_CAMERA_MOVES 152
#define HOW_FAR_TO_RIGHT_BEFORE_CAMERA_MOVES 153
#define HOW_FAR_TO_TOP_BEFORE_CAMERA_MOVES 115
#define HOW_FAR_TO_BOTTOM_BEFORE_CAMERA_MOVES 116

#define HORIZONTAL_RESOLUTION 320
#define VERTICAL_RESOLUTION 224
#define MAP_WIDTH 10240
#define MAP_HEIGHT 1280
#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

Sprite* player;

u16 ind = TILE_USER_INDEX;
Map* level_1_map;

Vect2D_f32 playerPosition = {FIX32(100), FIX32(100)};
fix32 playerVelocity = FIX32(8.5);

int current_camera_x = 100;
int current_camera_y = 100;

static void handleInput();
static void updateCamera();

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
    player = SPR_addSprite(&spr_player, playerPosition.x, playerPosition.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));

    JOY_setEventHandler(joyEvent);

    while (TRUE)
    {
        handleInput();
        updateCamera();
        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}

static void handleInput()
{
    u16 joy1Value = JOY_readJoypad(JOY_1);

    if (joy1Value & BUTTON_LEFT)
    {
        playerPosition.x -= playerVelocity;
        SPR_setAnim(player, ANIM_WALK_LEFT);
    }
    else if (joy1Value & BUTTON_RIGHT)
    {
        playerPosition.x += playerVelocity;
        SPR_setAnim(player, ANIM_WALK_RIGHT);
    }
    
    if (joy1Value & BUTTON_UP)
    {
        playerPosition.y -= playerVelocity;
        SPR_setAnim(player, ANIM_WALK_BACK);

    }
    else if (joy1Value & BUTTON_DOWN)
    {
        playerPosition.y += playerVelocity;
        SPR_setAnim(player, ANIM_WALK_FRONT);
    }

    if(!(joy1Value & BUTTON_LEFT) && !(joy1Value & BUTTON_RIGHT) && !(joy1Value & BUTTON_DOWN) && !(joy1Value & BUTTON_UP))
    {
        SPR_setAnimAndFrame(player, ANIM_IDLE_FRONT, 0);
        player->timer = 0;
    }
}

static void updateCamera()
{
    if (playerPosition.x < FIX32(0))
    {
        playerPosition.x = FIX32(0);
    }
    else if(playerPosition.x > FIX32(MAP_WIDTH - PLAYER_WIDTH))
    {
        playerPosition.x = FIX32(MAP_WIDTH - PLAYER_WIDTH);
    }

    if (playerPosition.y < FIX32(0))
    {
        playerPosition.y = FIX32(0);
    }
    else if(playerPosition.y > FIX32(MAP_HEIGHT - PLAYER_HEIGHT))
    {
        playerPosition.y = FIX32(MAP_HEIGHT - PLAYER_HEIGHT);
    }
    

    //position of player on map as a whole number
    int player_x_map_integer = fix32ToInt(playerPosition.x);
    int player_y_map_integer = fix32ToInt(playerPosition.y);

    //player position on screen
    int player_x_position_on_screen = player_x_map_integer - current_camera_x;
    int player_y_position_on_screen = player_y_map_integer - current_camera_y;

    //new camera position
    int new_camera_x;
    int new_camera_y;

    if (player_x_position_on_screen > HOW_FAR_TO_RIGHT_BEFORE_CAMERA_MOVES)
    {
        new_camera_x = player_x_map_integer - HOW_FAR_TO_RIGHT_BEFORE_CAMERA_MOVES;
    }
    else if (player_x_position_on_screen < HOW_FAR_TO_LEFT_BEFORE_CAMERA_MOVES)
    {
        new_camera_x = player_x_map_integer - HOW_FAR_TO_LEFT_BEFORE_CAMERA_MOVES;
    }
    else   
        new_camera_x = current_camera_x;
    
    if (player_y_position_on_screen > HOW_FAR_TO_BOTTOM_BEFORE_CAMERA_MOVES)
    {
        new_camera_y = player_y_map_integer - HOW_FAR_TO_BOTTOM_BEFORE_CAMERA_MOVES;
    }
    else if (player_y_position_on_screen < HOW_FAR_TO_TOP_BEFORE_CAMERA_MOVES)
    {
        new_camera_y = player_y_map_integer - HOW_FAR_TO_TOP_BEFORE_CAMERA_MOVES;
    }
    else   
        new_camera_y = current_camera_y;
    

    if (new_camera_x < 0)
    {
        new_camera_x = 0;
    }
    else if(new_camera_x > MAP_WIDTH - HORIZONTAL_RESOLUTION)
    {
        new_camera_x = MAP_WIDTH - HORIZONTAL_RESOLUTION;
    }

    if (new_camera_y < 0)
    {
        new_camera_y = 0;
    }
    else if(new_camera_y > MAP_HEIGHT - VERTICAL_RESOLUTION)
    {
        new_camera_y = MAP_HEIGHT - VERTICAL_RESOLUTION;
    }
    

    // Checks if camera needs to scrool
    if (new_camera_x != current_camera_x || new_camera_y != current_camera_y)
    {
        current_camera_x = new_camera_x;
        current_camera_y = new_camera_y;
        MAP_scrollTo(level_1_map, current_camera_x, current_camera_y);
    }

    

    SPR_setPosition(player, fix32ToInt(playerPosition.x) - new_camera_x, fix32ToInt(playerPosition.y) - new_camera_y);
}
