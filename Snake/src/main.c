#include <genesis.h>

#include "queue_Vect2D_u32.h"

u32 bugsEaten = 0;

#define PLAYABLE_GROUND_X_MIN 3
#define PLAYABLE_GROUND_X_MAX 37
#define PLAYABLE_GROUND_Y_MIN 3
#define PLAYABLE_GROUND_Y_MAX 25

enum Direction
{
    UP,
    Left,
    Down,
    Right
};
enum Direction currentDirection;

enum GridEntity
{
    Bug,
    Snake,
    Wall
};
enum GridEntity grid[40][28];

Vect2D_u32 screenCenterPosition = {20,14};
Vect2D_u32 currentPosition;

Vect2D_u32 currentBugPosition;

u16 elapsedTime = 0;
u16 timeToMove = 5;

bool movedByInputThisframe;

u32 RemapValue(u32 lowerBound, u32 upperBound, u32 value)
{
    return value % ((upperBound - lowerBound + 1) + lowerBound);
}

void myJoyHandler( u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		if (changed & BUTTON_UP & state)
		{
            currentDirection = UP;
		}
        else if (changed & BUTTON_DOWN & state)
		{
            currentDirection = Down;
		}
        else if (changed & BUTTON_LEFT & state)
		{
            currentDirection = Left;
		}
        else if (changed & BUTTON_RIGHT & state)
		{
            currentDirection = Right;
        }
	}	
    movedByInputThisframe = TRUE;
    moveSnake();
}

void spawnBug()
{
    Vect2D_u32 spawnPosition = 
    {
        RemapValue(PLAYABLE_GROUND_X_MIN, PLAYABLE_GROUND_X_MAX, random()),
        RemapValue(PLAYABLE_GROUND_Y_MIN, PLAYABLE_GROUND_Y_MAX, random())
    };
    currentBugPosition = spawnPosition;
    grid[currentBugPosition.x][currentBugPosition.y] = Bug;
}
void update()
{
   updateEntities();
}

void updateEntities()
{
    updateSnake();
    updateBug();
}

void updateSnake()
{
    elapsedTime += 1;

    if (elapsedTime <= timeToMove)
        return;

    elapsedTime = 0;

    moveSnake();

    if (currentPosition.x == currentBugPosition.x && currentPosition.y == currentBugPosition.y )
    {
        EatBug();
        spawnBug();
    }
}

void moveSnake()
{
    if (movedByInputThisframe)
        return;

    QUEUE_insert(currentPosition);

    if (QUEUE_size() > bugsEaten)
    {
       Vect2D_u32 taleSectionToRemove = QUEUE_deQueue();
       VDP_clearText(taleSectionToRemove.x, taleSectionToRemove.y, 1);
    }
    
    switch (currentDirection)
    {
        case UP:
            currentPosition.y -= 1;
            break;
        case Down:
            currentPosition.y += 1;
            break;
        case Left:
            currentPosition.x -= 1;
            break;
        case Right:
            currentPosition.x += 1;
            break;
        default:
            break;
    }

    Vect2D_u32 queueRange = QUEUE_getRange();

    VDP_drawText("O", currentPosition.x, currentPosition.y);
}


void EatBug()
{
    currentBugPosition.x = 0;
    bugsEaten++;
}

void updateBug()
{
    if (currentBugPosition.x == 0)
        return;

    VDP_drawText("H", currentBugPosition.x, currentBugPosition.y);
}

int main(u16 hard)
{
    JOY_init();
    JOY_setEventHandler(&myJoyHandler);

    currentPosition = screenCenterPosition;
    currentDirection = UP;

    bugsEaten = 3;

    spawnBug();

    char as[10];
    intToStr(QUEUE_getRear(),as,2);
    VDP_drawText(as,30,26);

    while(TRUE)
    {
        movedByInputThisframe = 0;
        // read input
        // move sprite
        // update score
        // draw current screen (logo, start screen, settings, game, gameover, credits...)

        update();

        // wait for screen refresh and do all SGDK VBlank tasks
        SYS_doVBlankProcess();
    }

    return 0;
}


