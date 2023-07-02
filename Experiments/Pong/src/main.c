#include <genesis.h>
#include <string.h>
#include "resources.h"

#define SCREEN_WIDTH screenWidth / 8
#define SCREEN_HEIGHT screenHeight / 8

#define LEFT_EDGE 0
#define RIGHT_EDGE 320
#define TOP_EDGE 0
#define BOTTOM_EDGE 224

Sprite* ball;

Vect2D_u16 initialPosition = {160, 200};

int ballPosX = 100;
int ballPosY = 100;
int ballVelX = 1;
int ballVelY = 1;
int ballWidth = 8;
int ballHeight = 8;
int moveBallMoveRate = 30;
int moveBallCurrentTime = 0;

Sprite* player;

int playerPosX = 144;
const int playerPosY = 200;
int playerVelX = 0;
const int playerWidth = 32;
const int playerHeight = 8;
const int playerUpdateTime = 30;
int playerCurrentUpdateTime = 0;

/*Score variables*/
int score = 0;
char labelScore[6] = "SCORE\0";
char strScore[4] = "0";

gameOn = FALSE;
char msgStart[22] = "PRESS START TO BEGIN!\0";
char msgReset[37] = "GAME OVER! PRESS START TO PLAY AGAIN.";

void showText(char s[]){
	VDP_drawText(s, 20 - strlen(s)/2 ,15);
}

void startGame(){
	score = 0;
	updateScoreDisplay();

	ballPosX = 0;
	ballPosY = 0;

	ballVelX = 1;
	ballVelY = 1;

	/*Clear the text from the screen*/
	VDP_clearTextArea(0,10,40,10);

	gameOn = TRUE;
}

void endGame(){
	showText(msgReset);
	gameOn = FALSE;
}

int sign(int x) {
    return (x > 0) - (x < 0);
}

void myJoyHandler( u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{

        if(state & BUTTON_START){
            if(!gameOn){
                startGame();    
            }
        }
		/*Set player velocity if left or right are pressed;
		 *set velocity to 0 if no direction is pressed */
		if (state & BUTTON_RIGHT)
		{
			playerVelX = 3;
		}
		else if (state & BUTTON_LEFT)
		{
			playerVelX = -3;
		} else{
			if( (changed & BUTTON_RIGHT) | (changed & BUTTON_LEFT) ){
				playerVelX = 0;
			}
		}
	}
}

void positionPlayer(){

    if (playerCurrentUpdateTime <= playerUpdateTime)
    {
        playerCurrentUpdateTime++;
        return;
    }
    playerCurrentUpdateTime = 0;

	/*Add the player's velocity to its position*/
	playerPosX += playerVelX;

	/*Keep the player within the bounds of the screen*/
	if(playerPosX < LEFT_EDGE) playerPosX = LEFT_EDGE;
	if(playerPosX + playerWidth > RIGHT_EDGE) playerPosX = RIGHT_EDGE - playerWidth;

	/*Let the Sprite engine position the sprite*/
	SPR_setPosition(player,playerPosX,playerPosY);
}

void moveBall()
{
    if (moveBallCurrentTime <= moveBallMoveRate)
    {
        moveBallCurrentTime++;
        return;
    }
    moveBallCurrentTime = 0;

    ballPosX += ballVelX;
    ballPosY += ballVelY;

    if(ballPosX < playerPosX + playerWidth && ballPosX + ballWidth > playerPosX){
        if(ballPosY < playerPosY + playerHeight && ballPosY + ballWidth >= playerPosY){
            //On collision, invert the velocity
            ballPosY = playerPosY - ballHeight - 1;
            ballVelY = -ballVelY;

            score++;
            updateScoreDisplay();

            if( score % 10 == 0)
            {
                ballVelX += sign(ballVelX);
                ballVelY += sign(ballVelY);
            }

        }
    }

    if(ballPosX < LEFT_EDGE)
    {
        ballPosX = LEFT_EDGE;
        ballVelX = -ballVelX;
    } 
    else if(ballPosX + ballWidth > RIGHT_EDGE)
    {
        ballPosX = RIGHT_EDGE - ballWidth;
        ballVelX = -ballVelX;
    }

    if(ballPosY < TOP_EDGE)
    {
        ballPosY = TOP_EDGE;
        ballVelY = -ballVelY;
    } 
    else if(ballPosY + ballHeight > BOTTOM_EDGE)
    {
        endGame();
    }

    SPR_setPosition(ball, ballPosX, ballPosY);
}

void updateScoreDisplay(){
	sprintf(strScore,"%d",score);
	VDP_clearText(1,2,3);
	VDP_drawText(strScore,1,2);
}

int main()
{
    JOY_init();
    JOY_setEventHandler( &myJoyHandler );

    SPR_init();

    ball = SPR_addSprite(&sprBall, initialPosition.x, initialPosition.y, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
    player = SPR_addSprite(&sprPaddle, playerPosX, playerPosY, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));

    PAL_setPalette(PAL1, bgTile.palette->data, DMA);
    VDP_loadTileSet(bgTile.tileset, 1, DMA);
    VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,1),0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    VDP_setTextPlane(BG_A);
    VDP_drawText(labelScore,1,1);

    showText(msgStart);
    updateScoreDisplay();

    while(1)
    {
        if(gameOn == TRUE){
		    moveBall();
		    positionPlayer();
	    }

        SPR_update();
        SYS_doVBlankProcess();
    }
    return (0);
}
