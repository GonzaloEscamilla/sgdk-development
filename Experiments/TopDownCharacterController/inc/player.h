#pragma once

#include <genesis.h>
#include <resources.h>
#include "../inc/collisions.h"
#include "../inc/level.h"
#include "../inc/coinsController.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32
#define PLAYER_HALF_WIDTH 16
#define PLAYER_HALF_HEIGHT 16

#define PLAYER_DEPENETRATION FIX32(0.1)
#define PLAYER_COLBOX_LEFT 9
#define PLAYER_COLBOX_RIGHT 23
#define PLAYER_COLBOX_TOP 18
#define PLAYER_COLBOX_BOTTOM 30

extern const BoxCollider characterBox;

typedef enum PlayerDirection
{
    None,
    Left,
    Right,
    Back,
    Front,
    BackLeft,
    BackRight,
    FrontLeft,
    FrontRight
} PlayerDirection;

typedef struct Player_
{
    Sprite* sprite;
    Vect2D_f32 position;
    Vect2D_f32 velocity;
    fix32 speed;
    fix32 diagonalSpeed;
    PlayerDirection currentDirection;
} Player;

extern Player player;

void PLAYER_Init();
void PLAYER_Update();
void PLAYER_InputChanged();
void PLAYER_checkCollisions();