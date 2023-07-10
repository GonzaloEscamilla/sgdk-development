#include "genesis.h"
#include <resources.h>
#include "../inc/player.h"
#include "../inc/inputs.h"
#include "../inc/collisions.h"
#include "../inc/camera.h"

u16 ind = TILE_USER_INDEX;
Map* current_level;

static void handleInput();
static void updateCamera();
void updatePlayerPosition();
void loadRoomByIndex(int roomIndex);
void loadRoom(MapDefinition* mapToLoad);

char info[10];

u8 currentRoomIndex = 0;

int main(bool hardReset)
{
    loadRoomByIndex(1);

    SPR_init();
    
    PAL_setPalette(PAL0, dungeon_pal0.data, DMA);
    
    SYS_doVBlankProcess();

    INPUT_Init();
    PLAYER_Init();
    CAMERA_Init(current_level);

    while (TRUE)
    {
        PLAYER_Update();
        CAMERA_update();

        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}

void loadRoomByIndex(int roomIndex)
{
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
    
    player.position.x = FIX32(140);
    player.position.y = FIX32(160);
    
    VDP_loadTileSet(&dungeon_tileset, ind, DMA);
    
    current_level = MAP_create(mapToLoad, BG_A, ind);
    MAP_scrollTo(current_level, 0, 0);
 
    //CAMERA_update();

    SPR_update();
    SYS_doVBlankProcess();

    u16 palette[32];
    memcpy(&palette[0], dungeon_pal0.data, 16);
    memcpy(&palette[16], spr_player.palette->data, 16);
    PAL_fadeIn(0, 32, palette, 30, FALSE);
}