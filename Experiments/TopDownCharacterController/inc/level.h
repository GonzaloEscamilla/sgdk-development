#pragma once

#include <genesis.h>
#include <resources.h>
#include "../inc/player.h"
#include "../inc/coinsController.h"

#define LEVEL_TILE_SIZE 16
#define WALKABLE_TILE 0
#define SOLID_TILE 1
#define CHANGEROOM_TILE 2
#define TRIGGER_TILE 3

typedef struct Level_
{
    MapDefinition* mapDefinition;
    u8* collisionMap;
} Level;

extern Level* currentLevel;
extern Map* currentLevelMap;

extern u16 VDPTilesFilled;
extern const u16 LENGHT_OF_LEVELCOL_ARRAY;
extern const u8 ROOM_A_COLLISIONS[280];
extern const u8 ROOM_B_COLLISIONS[280];

void LEVEL_Init();
void LEVEL_Load();
