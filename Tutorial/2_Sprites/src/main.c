#include "genesis.h"
#include <resources.h>

Sprite* player;
Sprite* animatedPlayer;
Sprite* myAnimatedBall;

const u16 ANIM_SOMETHING = 0; // Memory allocation on readonly parts of the program, small overhead due to value reading from memory

#define ANIM_STILL 0  // Macro, no allocation, no overhead, just code expansion
#define ANIM_IDLE 1
#define ANIM_WALK 2
#define ANIM_UPPER 3

#define ANIMATION_SWITCH_SPEED 60

int main(bool hardReset)
{
    SPR_init();
    
    PAL_setPalette(PAL2, our_sprite.palette->data, DMA);
    player = SPR_addSprite(&our_sprite, 0, 0, TILE_ATTR(PAL2, 0, FALSE, FALSE));

    PAL_setPalette(PAL3, myAnimatedSprite.palette->data, DMA);
    animatedPlayer = SPR_addSprite(&myAnimatedSprite, 10, 10, TILE_ATTR(PAL3, 0, FALSE, FALSE));
        
        
    PAL_setPalette(PAL1, myBall.palette->data, DMA);
    myAnimatedBall = SPR_addSprite(&myBall, 15, 15, TILE_ATTR(PAL1, 0, FALSE, FALSE));

    u16 ind = TILE_USER_INDEX;

    int animationCounter = 0;
    u16 currentAnimation = ANIM_IDLE;

    while (TRUE)
    {
        // Increment the animation counter
        animationCounter++;

        // Check if it's time to switch animations
        if (animationCounter >= ANIMATION_SWITCH_SPEED)
        {
            animationCounter = 0; // Reset the counter
            
            // Switch to the next animation
            currentAnimation++;
            if (currentAnimation > ANIM_UPPER)
            {
                currentAnimation = ANIM_IDLE;
            } // Wrap around if exceeding the maximum animation value
            
            // Set the current animation for the animatedPlayer sprite
            SPR_setAnim(animatedPlayer, currentAnimation);
        }

        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}
