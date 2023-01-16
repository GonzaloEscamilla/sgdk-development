#include <genesis.h>
#include <string.h>
#include "resources.h"

#define SCREEN_WIDTH screenWidth / 8
#define SCREEN_HEIGHT screenHeight / 8

#define LEFT_EDGE 0
#define RIGHT_EDGE 320
#define TOP_EDGE 0
#define BOTTOM_EDGE 224

 typedef struct {
    f32 X;
    bool JumpDown;
    bool JumpUp;
 } FrameInput;

Sprite* player;

fix32 playerPosX = 140.00;
fix32 playerPosY = 100.00;

Vect2D_f32 currentPosition;
Vect2D_f32 lastPosition;

/*
int playerVelX = 0;
const int playerWidth = 32;
const int playerHeight = 8;
*/

Vect2D_f32 velocity;
Vect2D_f32 rawMovement;
Vect2D_f32 lastPosition;

f32 currentHorizontalSpeed;
f32 currentVerticalSpeed;

FrameInput input;

bool jumpingThisFrame;
bool landingThisFrame;
bool grounded;

u32 framesSinceStart = 0;
f32 deltaTime = FIX32(0.016);

f32 _acceleration = FIX32(40.00);
f32 _moveClamp = FIX32(4.00);
f32 _deAccelerationStep = FIX32(0.5);
f32 _apexBonus = FIX32(2.00);

fix32 jumpEndEarlyGravityModifier = FIX32(3.00);
u32 lastJumpPressed = FIX32(0.000);
fix32 jumpHeight = FIX32(1.000);
bool endedJumpEarly = TRUE;
bool jumpThisFrame = FALSE;

fix32 fallClamp = -FIX32(40.00);
fix32 minFallSpeed = FIX32(80.00);
fix32 maxFallSpeed = FIX32(120.00);
fix32 fallSpeed = FIX32(0.00);

bool colDown = FALSE;

void myJoyHandler( u16 joy, u16 changed, u16 state)
{
    input.JumpUp = FALSE;
    input.JumpDown = FALSE;

	if (joy == JOY_1)
	{
        input.X = 0;

		if (state & BUTTON_RIGHT)
		{
            input.X = 1;
		}
		else if (state & BUTTON_LEFT)
		{
            input.X = -1;
		}
        if (BUTTON_UP)
		{
            input.JumpUp = !state;
            input.JumpDown = state;

            if (input.JumpDown)
            {
                lastJumpPressed = framesSinceStart;
            }
            
            //KLog_U1("Jump Up: ", input.JumpUp);
            //KLog_U1("Jump Down: ", input.JumpDown);
		}
	}
}



#pragma region Walking

    f32 moveTowards(f32 from, f32 to, f32 step)
    {
        if (abs(from - to) <= FIX32(0.1))
            return to;

        if (from > to)
            from -= step;

        if (from < to)
            from += step;

        return from;
    }

    void calculateVelocity()
    {
        f32 deltaX = lastPosition.x - currentPosition.x;
        f32 deltaY = lastPosition.y - currentPosition.y;

        if (deltaX < FIX32(0.00) )
            velocity.x = FIX32(1.00);
        else if(deltaX > FIX32(0.00))
            velocity.x = -FIX32(1.00);        
        else
            velocity.x = FIX32(0.00);

        if (deltaY < FIX32(0.00) )
            velocity.x = FIX32(1.00);
        else if(deltaY > FIX32(0.00))
            velocity.y = FIX32(1.00);        
        else
            velocity.y = -FIX32(0.00);

        lastPosition = currentPosition;

       // KLog_F1("Velocity.x: ", velocity.x);
       KLog_F1("Velocity.y: ", velocity.y);
    }

    void calculateWalk() {
        if (input.X != 0) {
            // Set horizontal move speed
            currentHorizontalSpeed += input.X * _acceleration * FIX32(0.01);
            //KLog_F1("CurrentHorizontalSpeed: ", currentHorizontalSpeed);

            // clamped by max frame movement
            
            //KLog_F1("LowerClampValue: ", -_moveClamp);
            //KLog_F1("HigherClampValue: ", _moveClamp);


            currentHorizontalSpeed = clamp(currentHorizontalSpeed, -_moveClamp, _moveClamp);
            
            //KLog_F1("CurrentHorizontalSpeedClamp: ", currentHorizontalSpeed);

            /*
            // Apply bonus at the apex of a jump
            var apexBonus = Mathf.Sign(Input.X) * _apexBonus * _apexPoint;
            _currentHorizontalSpeed += apexBonus * Time.deltaTime;
            */
        }
        else {
            // No input. Let's slow the character down
            currentHorizontalSpeed = moveTowards(currentHorizontalSpeed, 0, _deAccelerationStep);
           // KLog_F1("Deaceleration: ", currentHorizontalSpeed);
        }


        /*
        if (currentHorizontalSpeed > 0 && _colRight || _currentHorizontalSpeed < 0 && _colLeft) {
            // Don't walk through walls
            _currentHorizontalSpeed = 0;
        }
        */
    }

#pragma endregion 



void calculateJump()
{
    KLog_U1("Input.JumpDown ", input.JumpDown);
    KLog_U1("Input.JumpUp ", input.JumpUp);
    KLog_F1("endedJumpEarly ", endedJumpEarly);
    KLog_F1("velocity.y ", velocity.y);


    if (input.JumpDown)
    {
        currentVerticalSpeed = jumpHeight;
        endedJumpEarly = FALSE;
        KLog_U1("Endup Early: ", endedJumpEarly);
        jumpThisFrame = TRUE;
    }
    else
    {
        jumpThisFrame = TRUE;
    }
    
    if (!colDown && input.JumpUp && !endedJumpEarly && velocity.y > 0)
    {
        endedJumpEarly = TRUE;
        KLog_U1("Endup Early: ", endedJumpEarly);
    }
}

void calculateGravity()
{
    if (colDown) 
    {
        // Move out of the ground
        if (currentVerticalSpeed < 0) 
            currentVerticalSpeed = 0;
    }
    else {
        // Add downward force while ascending if we ended the jump early
        f32 fallSpeed = FIX32(0.00);
        if (endedJumpEarly && currentVerticalSpeed > 0)
        {
            fallSpeed = fallSpeed * jumpEndEarlyGravityModifier;
            KLog_F1("EndedUpEarly: ", fallSpeed);
        }
        else
        {
            fallSpeed = fallSpeed;
            KLog_F1("MaxJunp: ", fallSpeed);
        }
        

        // Fall
        currentVerticalSpeed -= fallSpeed * deltaTime;

        // Clamp
        if (currentVerticalSpeed < fallClamp) 
            currentVerticalSpeed = fallClamp;
    }
}

void updateCharacter()
{
    currentPosition.x += currentHorizontalSpeed;
    currentPosition.y -= currentVerticalSpeed;

    KLog_F1("Current X Position: ", currentPosition.x);
    KLog_F1("Current Y Position: ", currentPosition.y);

	/*Let the Sprite engine position the sprite*/
	SPR_setPosition(player, fix32ToInt(currentPosition.x), fix32ToInt(currentPosition.y));
}


void calculateCollisions()
{
    if (currentPosition.y >= FIX32(200.00))
    {
        colDown = TRUE;
    }
    else
        colDown = FALSE;
}

int main()
{
    JOY_init();
    JOY_setEventHandler( &myJoyHandler );

    SPR_init();

    PAL_setPalette(PAL1, bgTile.palette->data, DMA);
    VDP_loadTileSet(bgTile.tileset, 1, DMA);
    VDP_fillTileMapRect(BG_B, TILE_ATTR_FULL(PAL1,0,FALSE,FALSE,1),0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    currentPosition = (Vect2D_f32) { .x = FIX32(140.00), .y = FIX32(180.00)};
    lastPosition = currentPosition;

    PAL_setPalette(PAL2, &sprCharacter.palette->data, DMA);
    player = SPR_addSprite(&sprCharacter, currentPosition.x, currentPosition.y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    

    while(1)
    {
        calculateCollisions();    
        KLog_U1("Clo: ", colDown);    
        calculateVelocity();
        calculateWalk();
        calculateJump();
        calculateGravity();
        updateCharacter();
        

        framesSinceStart += 1;

        SPR_update();
        SYS_doVBlankProcess();
    }
    return (0);
}
