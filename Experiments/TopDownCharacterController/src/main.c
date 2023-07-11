#include "genesis.h"
#include <resources.h>
#include "../inc/player.h"
#include "../inc/inputs.h"
#include "../inc/collisions.h"
#include "../inc/camera.h"
#include "../inc/level.h"

char info[10];

int main(bool hardReset)
{
    LEVEL_Init();

    SPR_init();
    
    PAL_setPalette(PAL0, dungeon_pal0.data, DMA);
    
    SYS_doVBlankProcess();

    INPUT_Init();
    PLAYER_Init();
    CAMERA_Init(currentLevelMap);

    while (TRUE)
    {
        PLAYER_Update();
        CAMERA_update();

        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}