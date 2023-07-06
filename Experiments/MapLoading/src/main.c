#include "genesis.h"
#include <resources.h>

typedef enum 
{
    None,
    Left,
    Right,
    Up,
    Down
} PlayerDirection;

const u16 LENGHT_OF_LEVELCOL_ARRAY = 20;

#define WALKABLE_TILE 0
#define SOLID_TILE 1
#define CHANGEROOM_TILE 2
#define TRIGGER_TILE 3

const u8 ROOM_A_COLLISIONS[280] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
	2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1,
	1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 
};

const u8 ROOM_B_COLLISIONS[280] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1,
	1, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 
};

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
#define PLAYER_COLBOX_LEFT 10
#define PLAYER_COLBOX_RIGHT 23
#define PLAYER_COLBOX_TOP 18
#define PLAYER_COLBOX_BOTTOM 31

#define STOPTHRESHOLD FIX32(0.1)
#define ACCELERATION FIX32(0.6)
#define DECELERATION FIX32(0.3)
#define MAXSPEED FIX32(1.5)

Sprite* player;

u16 ind = TILE_USER_INDEX;
Map* current_level;

Vect2D_f32 playerPosition = {FIX32(100), FIX32(100)};
Vect2D_f32 playerVelocity = {FIX32(0.0f), FIX32(0.0f)};
Vect2D_s32 inputValue = {FIX32(0.0f), FIX32(0.0f)};
fix32 playerSpeed = FIX32(1.5);

PlayerDirection currentPlayerDirection = None;

int current_camera_x = 100;
int current_camera_y = 100;

static void handleInput();
static void updateCamera();
void updatePlayerPosition();
void checkCollisions();
void loadRoomByIndex(int roomIndex);
void loadRoom(MapDefinition* mapToLoad);

char info[10];

u8 currentRoomIndex = 0;

static void joyEvent(u16 joy, u16 changed, u16 state)
{

}

int main(bool hardReset)
{
    loadRoomByIndex(0);

    SPR_init();
    
    PAL_setPalette(PAL0, dungeon_pal0.data, DMA);
    PAL_setPalette(PAL1, spr_player.palette->data, DMA);
    player = SPR_addSprite(&spr_player, playerPosition.x, playerPosition.y, TILE_ATTR(PAL1, 0, FALSE, FALSE));

    JOY_setEventHandler(joyEvent);

    while (TRUE)
    {
        handleInput();
        checkCollisions();
        updateCamera();

        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}

void loadRoomByIndex(int roomIndex)
{
    kprintf("Here");


    if (roomIndex == 0)
    {
        loadRoom(&map_room_four_doors);
    }
    if (roomIndex == 1)
    {
        loadRoom(&map_room_two_doors);
    }
}

void loadRoom(MapDefinition* mapToLoad)
{
    // Fade out all graphics to Black
    PAL_fadeOutAll(30, FALSE);

    //VDP_clearPlane(BG_A, TRUE);
    //VDP_clearSprites();

    if (current_level != NULL)
        MAP_release(current_level);

    playerPosition.x = FIX32(140);
    playerPosition.y = FIX32(160);

    VDP_loadTileSet(&dungeon_tileset, ind, DMA);
    
    current_level = MAP_create(mapToLoad, BG_A, ind);
    MAP_scrollTo(current_level, 0, 0);
 
    updateCamera();

    SPR_update();
    SYS_doVBlankProcess();

    u16 palette[32];
    memcpy(&palette[0], dungeon_pal0.data, 16);
    memcpy(&palette[16], spr_player.palette->data, 16);
    PAL_fadeIn(0, 32, palette, 30, FALSE);
}

static void handleInput()
{
    u16 joy1Value = JOY_readJoypad(JOY_1);


    currentPlayerDirection = None;

    if (joy1Value & BUTTON_LEFT)
    {
        //inputValue.x = clamp(inputValue.x - ACCELERATION, -MAXSPEED, MAXSPEED);
        //playerPosition.x -= playerSpeed;
        
        currentPlayerDirection = Left;
        //SPR_setAnim(player, ANIM_WALK_LEFT);
    }
    else if (joy1Value & BUTTON_RIGHT)
    {
        //inputValue.x = clamp(inputValue.x + ACCELERATION, -MAXSPEED, MAXSPEED);
        currentPlayerDirection = Right;
        //SPR_setAnim(player, ANIM_WALK_RIGHT);
    }/* else
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
    } */
    
    if (joy1Value & BUTTON_UP)
    {
        //inputValue.y = clamp(inputValue.y - ACCELERATION, -MAXSPEED, MAXSPEED);
        currentPlayerDirection = Up;
        
        //SPR_setAnim(player, ANIM_WALK_BACK);
    }
    else if (joy1Value & BUTTON_DOWN)
    {
        //inputValue.y = clamp(inputValue.y + ACCELERATION, -MAXSPEED, MAXSPEED);
        currentPlayerDirection = Down;
        //SPR_setAnim(player, ANIM_WALK_FRONT);
    }
    /* else
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
    } */

    if(!(joy1Value & BUTTON_LEFT) && !(joy1Value & BUTTON_RIGHT) && !(joy1Value & BUTTON_DOWN) && !(joy1Value & BUTTON_UP))
    {
        //SPR_setAnimAndFrame(player, ANIM_IDLE_FRONT, 0);
        //player->timer = 0;
    }
}

void updatePlayerPosition()
{
    //playerPosition.x += inputValue.x/** playerSpeed*/ ;
    //playerPosition.y += inputValue.y /*playerSpeed*/ ;
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
        MAP_scrollTo(current_level, current_camera_x, current_camera_y);
    }

    //kprintf("PlayerPosition: x = %ld, y = %ld\n", playerPosition.x, playerPosition.y);

    kprintf("Sprite Set Position");

    SPR_setPosition(player, fix32ToInt(playerPosition.x) - new_camera_x, fix32ToInt(playerPosition.y) - new_camera_y);
}

void checkCollisions()
{
    s16 player_left_collision_coord;
    s16 player_right_collision_coord;
    s16 player_top_collision_coord = fix32ToInt(playerPosition.y) + PLAYER_COLBOX_TOP;
    s16 player_bottom_collision_coord = fix32ToInt(playerPosition.y) + PLAYER_COLBOX_BOTTOM;

    s16 xtilecoord_left_collision_player;
    s16 xtilecoord_right_collision_player;
    s16 ytilecoord_top_collision_player = player_top_collision_coord >> 4;
    s16 ytilecoord_bottom_collision_player = player_bottom_collision_coord >> 4;

    u16 array_index_topleft_colbox;
    u16 array_index_topright_colbox;
    u16 array_index_bottomleft_colbox;
    u16 array_index_bottomright_colbox;

    u8 tile_collision_type_topleft = 0;
    u8 tile_collision_type_topright = 0;
    u8 tile_collision_type_bottomleft = 0;
    u8 tile_collision_type_bottomright = 0;

    s16 blocked_coord;

    sprintf(info, "%10i", tile_collision_type_bottomright );
    VDP_drawTextBG(BG_A, info, 28, 5);

    switch (currentPlayerDirection)
    {
        case Left:
        {
            playerPosition.x -= playerSpeed;
            player_left_collision_coord = fix32ToInt(playerPosition.x) + PLAYER_COLBOX_LEFT;
            xtilecoord_left_collision_player = player_left_collision_coord >> 4;
            array_index_topleft_colbox = xtilecoord_left_collision_player + (ytilecoord_top_collision_player * LENGHT_OF_LEVELCOL_ARRAY);
            tile_collision_type_topleft = ROOM_A_COLLISIONS[array_index_topleft_colbox];
            
            array_index_bottomleft_colbox = xtilecoord_left_collision_player + (ytilecoord_bottom_collision_player * LENGHT_OF_LEVELCOL_ARRAY);
            tile_collision_type_bottomleft = ROOM_A_COLLISIONS[array_index_bottomleft_colbox];

            if (tile_collision_type_topleft == SOLID_TILE || tile_collision_type_bottomleft == SOLID_TILE)
            {
                blocked_coord = (xtilecoord_left_collision_player << 4) + 16 - PLAYER_COLBOX_LEFT;
                playerPosition.x = intToFix32(blocked_coord);
            }

            break;
        }
        case Right:
        {
            playerPosition.x += playerSpeed;
            
            player_right_collision_coord = fix32ToInt(playerPosition.x) + PLAYER_COLBOX_RIGHT;
            xtilecoord_right_collision_player = player_right_collision_coord >> 4;
            array_index_topright_colbox = xtilecoord_right_collision_player + (ytilecoord_top_collision_player * LENGHT_OF_LEVELCOL_ARRAY);
            array_index_bottomright_colbox = xtilecoord_right_collision_player + (ytilecoord_bottom_collision_player * LENGHT_OF_LEVELCOL_ARRAY);
            
            tile_collision_type_topright = ROOM_A_COLLISIONS[array_index_topright_colbox];
            tile_collision_type_bottomright = ROOM_A_COLLISIONS[array_index_bottomright_colbox];

            if (tile_collision_type_topright == SOLID_TILE || tile_collision_type_bottomright == SOLID_TILE)
            {
                blocked_coord = (xtilecoord_right_collision_player << 4) - PLAYER_COLBOX_RIGHT;
                playerPosition.x = intToFix32(blocked_coord);
                playerPosition.x -= FIX32(0.1);
            }
            break;
        }
        case Up:
        {
            playerPosition.y -= playerSpeed;

            player_top_collision_coord = fix32ToInt(playerPosition.y) + PLAYER_COLBOX_TOP;
            ytilecoord_top_collision_player = player_top_collision_coord >> 4;

            player_left_collision_coord = fix32ToInt(playerPosition.x) + PLAYER_COLBOX_LEFT;
            xtilecoord_left_collision_player = player_left_collision_coord >> 4;
            array_index_topleft_colbox = xtilecoord_left_collision_player + (ytilecoord_top_collision_player * LENGHT_OF_LEVELCOL_ARRAY);
            tile_collision_type_topleft = ROOM_A_COLLISIONS[array_index_topleft_colbox];

            player_right_collision_coord = fix32ToInt(playerPosition.x) + PLAYER_COLBOX_RIGHT;
            xtilecoord_right_collision_player = player_right_collision_coord >> 4;
            array_index_topright_colbox = xtilecoord_right_collision_player + (ytilecoord_top_collision_player * LENGHT_OF_LEVELCOL_ARRAY);
            tile_collision_type_topright = ROOM_A_COLLISIONS[array_index_topright_colbox];

            if (tile_collision_type_topleft == SOLID_TILE || tile_collision_type_topright == SOLID_TILE)
            {
                blocked_coord = (ytilecoord_top_collision_player << 4) + 16  - PLAYER_COLBOX_TOP;
                playerPosition.y = intToFix32(blocked_coord);
            }

            break;
        }
        case Down:
        {
            playerPosition.y += playerSpeed;

            player_bottom_collision_coord = fix32ToInt(playerPosition.y) + PLAYER_COLBOX_BOTTOM;
            ytilecoord_bottom_collision_player = player_bottom_collision_coord >> 4;

            player_left_collision_coord = fix32ToInt(playerPosition.x) + PLAYER_COLBOX_LEFT;
            xtilecoord_left_collision_player = player_left_collision_coord >> 4;
            array_index_bottomleft_colbox = xtilecoord_left_collision_player + (ytilecoord_bottom_collision_player * LENGHT_OF_LEVELCOL_ARRAY);
            tile_collision_type_bottomleft = ROOM_A_COLLISIONS[array_index_bottomleft_colbox];

            player_right_collision_coord = fix32ToInt(playerPosition.x) + PLAYER_COLBOX_RIGHT;
            xtilecoord_right_collision_player = player_right_collision_coord >> 4;
            array_index_bottomright_colbox = xtilecoord_right_collision_player + (ytilecoord_bottom_collision_player * LENGHT_OF_LEVELCOL_ARRAY);
            tile_collision_type_bottomright = ROOM_A_COLLISIONS[array_index_bottomright_colbox];

            if (tile_collision_type_bottomleft == SOLID_TILE || tile_collision_type_bottomright == SOLID_TILE)
            {
                blocked_coord = (ytilecoord_bottom_collision_player << 4) - PLAYER_COLBOX_BOTTOM;
                playerPosition.y = intToFix32(blocked_coord);
                playerPosition.y -= FIX32(0.1); 
            }
            break;
        }
        default:
            break;
    }

    if (tile_collision_type_topleft == CHANGEROOM_TILE || 
        tile_collision_type_topright == CHANGEROOM_TILE || 
        tile_collision_type_bottomleft == CHANGEROOM_TILE || 
        tile_collision_type_bottomright == CHANGEROOM_TILE)
    {
        loadRoomByIndex(1);
    }
}
