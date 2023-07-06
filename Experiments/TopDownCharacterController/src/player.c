#include "../inc/player.h"

#define ANIMATION_SWITCH_SPEED 60
#define ANIM_IDLE_FRONT 0
#define ANIM_IDLE_BACK 1
#define ANIM_IDLE_RIGHT 2
#define ANIM_IDLE_LEFT 3
#define ANIM_WALK_FRONT 4
#define ANIM_WALK_BACK 5
#define ANIM_WALK_RIGHT 6
#define ANIM_WALK_LEFT 7

Player player;

void PLAYER_Init()
{
    player.velocity.x = FIX32(0.0f);
    player.velocity.y = FIX32(0.0f);
    player.speed = FIX32(1.5);
}