#ifndef __SAVES_H__
#define __SAVES_H__

#include "entity.h"
#include "simple_json.h"

/**
 * @brief Saves the players stats to a save file
 * @param player The player entity
 */
void save_game(Entity player);


/**
 * @brief loads a previously saved game
 * @return NULL on error, else the json of the game save
 */
int *load_game();

#endif
