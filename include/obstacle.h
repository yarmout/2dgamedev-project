#ifndef __OBSTACLES_H__
#define __OBSTACLES_H__

#include "entity.h"

/**
 * @brief spawn a obstacle entity
 * @param position the screen position to spawn the obstacle at
 * @param sprite_filename the location and filename of the sprite image to use
 * @return NULL on error, or a pointer to a new obstacle entity
 */
Entity *obstacle_spawn(Vector2D position, char *sprite_filename, float frameWidth, float frameHeight, int framesPerLine, float frameRate, int frameCount);

#endif
