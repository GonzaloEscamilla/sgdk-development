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
#define MAP_WIDTH 320
#define MAP_HEIGHT 224
#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

#define STOPTHRESHOLD FIX32(0.1)
#define ACCELERATION FIX32(0.6)
#define DECELERATION FIX32(0.3)
#define MAXSPEED FIX32(1.5)

Sprite* player;

u16 ind = TILE_USER_INDEX;
Map* level_1_map;

Vect2D_f32 playerPosition = {FIX32(100), FIX32(100)};
Vect2D_f32 playerVelocity = {FIX32(0.0f), FIX32(0.0f)};
Vect2D_s32 inputValue = {FIX32(0.0f), FIX32(0.0f)};
fix32 playerSpeed = FIX32(1);

int current_camera_x = 100;
int current_camera_y = 100;

static void handleInput();
static void updateCamera();
void updatePlayerPosition();

static void joyEvent(u16 joy, u16 changed, u16 state)
{

}

int main(bool hardReset)
{
    VDP_loadTileSet(&myTileset, ind, DMA);
    level_1_map = MAP_create(&myOwnMap, BG_A, ind);
    PAL_setPalette(PAL0, myPalette.data, DMA);
    MAP_scrollTo(level_1_map, 0, 768);

    SPR_init();
    
    PAL_setPalette(PAL1, spr_player.palette->data, DMA);
    player = SPR_addSprite(&spr_player, playerPosition.x, playerPosition.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));

    JOY_setEventHandler(joyEvent);

    while (TRUE)
    {
        handleInput();
        updatePlayerPosition();
        updateCamera();
        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}

bool isDiagonale = FALSE;

static void handleInput()
{
    u16 joy1Value = JOY_readJoypad(JOY_1);

    kprintf("Position: x = %ld, y = %ld\n", inputValue.x, inputValue.y);


    if (joy1Value & BUTTON_LEFT)
    {
        inputValue.x = clamp(inputValue.x - ACCELERATION, -MAXSPEED, MAXSPEED);

        //playerPosition.x -= playerSpeed;
        SPR_setAnim(player, ANIM_WALK_LEFT);
    }
    else if (joy1Value & BUTTON_RIGHT)
    {
        inputValue.x = clamp(inputValue.x + ACCELERATION, -MAXSPEED, MAXSPEED);
        
        //playerPosition.x += playerSpeed;
        SPR_setAnim(player, ANIM_WALK_RIGHT);
    }else
    {
        if (inputValue.x > STOPTHRESHOLD)
        {
            inputValue.x -= DECELERATION;
        }
        else if (inputValue.x < -STOPTHRESHOLD)
        {
            inputValue.x += DECELERATION;
        }
        else
        {
            inputValue.x = FIX32(0);
        }
    }
    
    if (joy1Value & BUTTON_UP)
    {
        inputValue.y = clamp(inputValue.y - ACCELERATION, -MAXSPEED, MAXSPEED);
        //playerPosition.y -= playerSpeed;
        SPR_setAnim(player, ANIM_WALK_BACK);
    }
    else if (joy1Value & BUTTON_DOWN)
    {
        inputValue.y = clamp(inputValue.y + ACCELERATION, -MAXSPEED, MAXSPEED);
        //playerPosition.y += playerSpeed;
        SPR_setAnim(player, ANIM_WALK_FRONT);
    }
    else
    {
        if (inputValue.y > STOPTHRESHOLD)
        {
            inputValue.y -= DECELERATION;
        }
        else if (inputValue.y < -STOPTHRESHOLD)
        {
            inputValue.y += DECELERATION;
        }
        else
        {
            inputValue.y = FIX32(0);
        }
    }

    if(!(joy1Value & BUTTON_LEFT) && !(joy1Value & BUTTON_RIGHT) && !(joy1Value & BUTTON_DOWN) && !(joy1Value & BUTTON_UP))
    {
        //SPR_setAnimAndFrame(player, ANIM_IDLE_FRONT, 0);
        player->timer = 0;
    }
}

void updatePlayerPosition()
{
    playerPosition.x += inputValue.x/** playerSpeed*/ ;
    playerPosition.y += inputValue.y /*playerSpeed*/ ;
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

    //kprintf("PlayerPosition: x = %ld, y = %ld\n", playerPosition.x, playerPosition.y);

    SPR_setPosition(player, fix32ToInt(playerPosition.x) - new_camera_x, fix32ToInt(playerPosition.y) - new_camera_y);
}
