#include "../inc/camera.h"
#include "../inc/player.h"

static Map* currentLevel;

int current_camera_x = 100;
int current_camera_y = 100;

void CAMERA_Init(Map* level)
{
    currentLevel = level;
}

void CAMERA_update()
{
    if (player.position.x < FIX32(0))
    {
        player.position.x = FIX32(0);
    }
    else if(player.position.x > FIX32(MAP_WIDTH - PLAYER_WIDTH))
    {
        player.position.x = FIX32(MAP_WIDTH - PLAYER_WIDTH);
    }

    if (player.position.y < FIX32(0))
    {
        player.position.y = FIX32(0);
    }
    else if(player.position.y > FIX32(MAP_HEIGHT - PLAYER_HEIGHT))
    {
        player.position.y = FIX32(MAP_HEIGHT - PLAYER_HEIGHT);
    }
    
    //position of player on map as a whole number
    int player_x_map_integer = fix32ToInt(player.position.x);
    int player_y_map_integer = fix32ToInt(player.position.y);

    //player position on screen
    int player_x_position_on_screen = player_x_map_integer - current_camera_x;
    int player_y_position_on_screen = player_y_map_integer - current_camera_y;

    //new camera position
    int new_camera_x;
    int new_camera_y;

    if (player_x_position_on_screen > HOW_FAR_TO_RIGHT_BEFORE_CAMERA_MOVES)
    {
        new_camera_x = player_x_map_integer - HOW_FAR_TO_RIGHT_BEFORE_CAMERA_MOVES;
    }
    else if (player_x_position_on_screen < HOW_FAR_TO_LEFT_BEFORE_CAMERA_MOVES)
    {
        new_camera_x = player_x_map_integer - HOW_FAR_TO_LEFT_BEFORE_CAMERA_MOVES;
    }
    else   
        new_camera_x = current_camera_x;
    
    if (player_y_position_on_screen > HOW_FAR_TO_BOTTOM_BEFORE_CAMERA_MOVES)
    {
        new_camera_y = player_y_map_integer - HOW_FAR_TO_BOTTOM_BEFORE_CAMERA_MOVES;
    }
    else if (player_y_position_on_screen < HOW_FAR_TO_TOP_BEFORE_CAMERA_MOVES)
    {
        new_camera_y = player_y_map_integer - HOW_FAR_TO_TOP_BEFORE_CAMERA_MOVES;
    }
    else   
        new_camera_y = current_camera_y;
    

    if (new_camera_x < 0)
    {
        new_camera_x = 0;
    }
    else if(new_camera_x > MAP_WIDTH - HORIZONTAL_RESOLUTION)
    {
        new_camera_x = MAP_WIDTH - HORIZONTAL_RESOLUTION;
    }

    if (new_camera_y < 0)
    {
        new_camera_y = 0;
    }
    else if(new_camera_y > MAP_HEIGHT - VERTICAL_RESOLUTION)
    {
        new_camera_y = MAP_HEIGHT - VERTICAL_RESOLUTION;
    }
    
    // Checks if camera needs to scrool
    if (new_camera_x != current_camera_x || new_camera_y != current_camera_y)
    {
        current_camera_x = new_camera_x;
        current_camera_y = new_camera_y;
        MAP_scrollTo(currentLevel, current_camera_x, current_camera_y);
    }

    //kprintf("PlayerPosition: x = %ld, y = %ld\n", playerPosition.x, playerPosition.y);

    SPR_setPosition(player.sprite, fix32ToInt(player.position.x) - new_camera_x, fix32ToInt(player.position.y) - new_camera_y);
}
