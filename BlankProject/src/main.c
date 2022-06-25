#include <genesis.h>


u16 positionX = 20;
u16 positionY = 20;

u16 deltaTime = 0;

fix32 currentXPosition = 0;
int currentXPositionInt = 0;

fix32 initialPosition = FIX32(10.000);
fix32 finalPosition = FIX32(20.000);

u16 timeAmount = 120;


fix32 fix32EaseOutTween(fix32 start, fix32 distance, u16 duration, s16 timer){
    u32 brad = 256 * timer / duration;

    return fix32Add(fix32Mul(distance, sinFix32(brad)), start);
}

void myJoyHandler( u16 joy, u16 changed, u16 state)
{


	if (joy == JOY_1)
	{
        VDP_clearText(positionX, positionY, 1);

		if (changed & BUTTON_UP & state)
		{
            positionY -= 1;
		}
        else if (changed & BUTTON_DOWN & state)
		{
            positionY += 1;
		}
        else if (changed & BUTTON_LEFT & state)
		{
            positionX -= 1;
		}
        else if (changed & BUTTON_RIGHT & state)
		{
            positionX += 1;
        }

        VDP_drawText("O", positionX, positionY);
	}	
}

int main(u16 hard)
{
    JOY_init();
    JOY_setEventHandler(&myJoyHandler);

    char xPositionStr[10];
    char yPositionStr[10];

    while(TRUE)
    {
        // read input
        // move sprite
        // update score
        // draw current screen (logo, start screen, settings, game, gameover, credits...)

        // wait for screen refresh and do all SGDK VBlank tasks
        deltaTime += 1;
        if (deltaTime >= 120)
            deltaTime = 0;
        
        VDP_clearText(currentXPositionInt, 20, 1);


        currentXPosition = fix32EaseOutTween(initialPosition, finalPosition, timeAmount, deltaTime);
        currentXPositionInt = fix32ToRoundedInt(currentXPosition);

        uintToStr(positionX, xPositionStr, 2);
        uintToStr(positionY, yPositionStr, 2);

        VDP_drawText("(", 1, 2);
        VDP_drawText(xPositionStr, 2, 2);
        VDP_drawText(":", 4, 2);

        VDP_drawText(yPositionStr, 5, 2);


        VDP_drawText("8", currentXPositionInt, 20);
        

        SYS_doVBlankProcess();
    }


    return 0;
}


