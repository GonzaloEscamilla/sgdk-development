#include <genesis.h>

#include "queue_Vect2D_u32.h"

#define SCREEN_TILE_WIDTH 40
#define SCREEN_TILE_HEIGHT 28

#define PLAYABLE_GROUND_X_MIN 3
#define PLAYABLE_GROUND_X_MAX 37
#define PLAYABLE_GROUND_Y_MIN 3
#define PLAYABLE_GROUND_Y_MAX 25

#define INITIAL_SNAKE_SIZE 3

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
    Empty,
    Bug,
    Snake,
    Wall
};
enum GridEntity grid[40][28];

Vect2D_u32 screenCenterPosition = {20,14};
Vect2D_u32 currentPosition;

Vect2D_u32 currentBugPosition;

u32 bugsEaten = 0;
u32 score = 0;
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
		if (changed & BUTTON_UP & state && currentDirection != Down )
		{
            currentDirection = UP;
		}
        else if (changed & BUTTON_DOWN & state && currentDirection != UP)
		{
            currentDirection = Down;
		}
        else if (changed & BUTTON_LEFT & state && currentDirection != Right)
		{
            currentDirection = Left;
		}
        else if (changed & BUTTON_RIGHT & state && currentDirection != Left)
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

    if (spawnPosition.x == 0 || spawnPosition.y == 0 || grid[spawnPosition.x][spawnPosition.y] == Snake)
    {
        spawnBug();
        return;
    }

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

bool checkIsDead()
{   
    if (grid[currentPosition.x][currentPosition.y] == Wall ||
        grid[currentPosition.x][currentPosition.y] == Snake )
        return TRUE;

    return FALSE;    
}

void moveSnake()
{
    if (movedByInputThisframe)
        return;

    if (checkIsDead())
    {
        Die();
        return;
    }

    QUEUE_insert(currentPosition);
    grid[currentPosition.x][currentPosition.y] = Snake;

    if (QUEUE_size() > bugsEaten)
    {
       Vect2D_u32 taleSectionToRemove = QUEUE_deQueue();
       grid[taleSectionToRemove.x][taleSectionToRemove.y] = Empty;
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

void Die()
{
    for (u32 i = QUEUE_getRange().x; i <= QUEUE_getRange().y; i++)
    {
        VDP_clearText(QUEUE_getElementAt(i).x, QUEUE_getElementAt(i).y,1);
        grid[QUEUE_getElementAt(i).x][QUEUE_getElementAt(i).y] = Empty;
    }

    VDP_resetScreen();

    VDP_clearText(currentBugPosition.x, currentBugPosition.y,1);
    grid[currentBugPosition.x][currentBugPosition.y] = Empty;
    
    QUEUE_clean();

    initLevel();
}

void EatBug()
{
    currentBugPosition.x = 0;
    bugsEaten++;
    score += 1;
    drawScore();
}

void updateBug()
{
    if (currentBugPosition.x == 0)
        return;

    VDP_drawText("H", currentBugPosition.x, currentBugPosition.y);
}

void drawScore()
{
    VDP_drawText("Score:", 15,0);
    
    char scoreStr[10];
    uintToStr(score,scoreStr,3);
    VDP_drawText(scoreStr, 21, 0);
    
}

void initWall()
{
    for (int i = 0; i < SCREEN_TILE_WIDTH; i++)
    {
        VDP_drawText("-", i, 0);
        grid[i][0] = Wall;

        VDP_drawText("-", i, SCREEN_TILE_HEIGHT - 1);
        grid[i][SCREEN_TILE_HEIGHT - 1] = Wall;
    }
    for (int i = 0; i < SCREEN_TILE_HEIGHT; i++)
    {
        VDP_drawText("|",0 , i);
        grid[0][i] = Wall;

        VDP_drawText("|",SCREEN_TILE_WIDTH - 1 , i);
        grid[SCREEN_TILE_WIDTH - 1][i] = Wall;
    }

    VDP_drawText("+",0,0);
    VDP_drawText("+", SCREEN_TILE_WIDTH - 1, 0);
    VDP_drawText("+", SCREEN_TILE_WIDTH - 1, SCREEN_TILE_HEIGHT - 1);
    VDP_drawText("+", 0, SCREEN_TILE_HEIGHT - 1);
}

void initLevel()
{
    currentPosition = screenCenterPosition;
    currentDirection = UP;

    bugsEaten = INITIAL_SNAKE_SIZE;

    spawnBug();

    initWall();

    score = 0;
    drawScore();
}

int main(u16 hard)
{
    JOY_init();
    JOY_setEventHandler(&myJoyHandler);

    initLevel();


    while(TRUE)
    {
        movedByInputThisframe = FALSE;
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


