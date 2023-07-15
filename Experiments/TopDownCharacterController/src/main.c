#include "genesis.h"
#include <resources.h>
#include "../inc/player.h"
#include "../inc/inputs.h"
#include "../inc/collisions.h"
#include "../inc/camera.h"
#include "../inc/level.h"
#include "../inc/coinsController.h"

char info[10];

int main(bool hardReset)
{
    LEVEL_Init();
    
    SYS_doVBlankProcess();

    INPUT_Init();
    PLAYER_Init();
    CAMERA_Init(currentLevelMap);
    
    KLog_U1("Free RAM: ",MEM_getFree());

    while (TRUE)
    {
        PLAYER_Update();
        CAMERA_update();
        //COINS_Update();a
        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}