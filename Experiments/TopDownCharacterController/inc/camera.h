#pragma once

#include "genesis.h"
#include <resources.h>

#define HOW_FAR_TO_LEFT_BEFORE_CAMERA_MOVES 152
#define HOW_FAR_TO_RIGHT_BEFORE_CAMERA_MOVES 153
#define HOW_FAR_TO_TOP_BEFORE_CAMERA_MOVES 115
#define HOW_FAR_TO_BOTTOM_BEFORE_CAMERA_MOVES 116

#define HORIZONTAL_RESOLUTION 320
#define VERTICAL_RESOLUTION 224
#define MAP_WIDTH 320
#define MAP_HEIGHT 224

extern int current_camera_x;
extern int current_camera_y;

void CAMERA_Init(Map* level);
void CAMERA_update();