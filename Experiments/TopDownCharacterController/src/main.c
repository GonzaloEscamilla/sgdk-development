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