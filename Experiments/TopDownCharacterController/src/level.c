#include "../inc/level.h"


#define WALKABLE_TILE 0
#define SOLID_TILE 1
#define CHANGEROOM_TILE 2
#define TRIGGER_TILE 3

u16 VDPTilesFilled = TILE_USER_INDEX;

Level* currentLevel;
Map* currentLevelMap;

const Level levelOne = 
{
    .mapDefinition = &map_room_four_doors,
	.collisionMap = &ROOM_A_COLLISIONS
};

const Level levelTwo = 
{
    .mapDefinition = &map_room_two_doors,
	.collisionMap = &ROOM_B_COLLISIONS
};

void LEVEL_Init()
{
    SPR_init();

	COINS_Init();

    PAL_setPalette(PAL0, dungeon_pal0.data, DMA); //TBD

	currentLevel = &levelTwo;

	LEVEL_Load(currentLevel);

	//Note: Spawn Initial Coins
	Coin* newCoin = COINS_Spawn(50, 50);
	COINS_Spawn(260, 50);
	COINS_Spawn(50, 180);
	COINS_Spawn(260, 180);

	Vect2D_s16 from = newVector2D_s16(50,50);
	Vect2D_s32 to = newVector2D_s32(250,50);

	Tweener* newTweener = TWEEN_Move1D(newCoin->sprite, from, EASE_DIRECTION_RIGHT, 120, FIX16(5.34));
	TWEEN_SetEase(newTweener, EaseLinear);
	TWEEN_SetCompletedCallback(newTweener, &TWEEN_Kill);
	//newTweener->Completed = (void*)&TWEEN_Kill; this works also
}

void LEVEL_Load(Level* level)
{
	 // Fade out all graphics to Black
    PAL_fadeOutAll(30, FALSE);

    if (currentLevelMap != NULL)
        MAP_release(currentLevelMap);
    
    player.position.x = FIX32(140);
    player.position.y = FIX32(160);
    
    VDP_loadTileSet(&dungeon_tileset, VDPTilesFilled, DMA);
    
    currentLevelMap = MAP_create(level->mapDefinition, BG_A, VDPTilesFilled);
    MAP_scrollTo(currentLevelMap, 0, 0);
 
    //CAMERA_update();

    SPR_update();
    SYS_doVBlankProcess();

    u16 palette[48];
    memcpy(&palette[0], dungeon_pal0.data, 16);
    memcpy(&palette[16], spr_player.palette->data, 16);
    memcpy(&palette[32], spr_coin.palette->data, 16);
    PAL_fadeIn(0, 48, palette, 30, FALSE);
}

const u16 LENGHT_OF_LEVELCOL_ARRAY = 20;

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
	1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
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
	1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 
};