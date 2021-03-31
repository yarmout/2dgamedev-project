#ifndef __POWER_UP_H__
#define __POWER_UP_H__

#include "entity.h"

/**
 * @brief spawn a power up entity
 * @param position the screen position to spawn the power up at
 * @param sprite_filename the location and filename of the sprite image to use
 * @return NULL on error, or a pointer to a new power up entity
 */
Entity *pickup_spawn(Vector2D position, char *sprite_filename, char *pick_up_type, float frameWidth, float frameHeight, int framesPerLine, float frameRate, int frameCount);


#endif
