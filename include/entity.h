#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include "shape.h"

typedef struct Entity_s
{
    Bool       _inuse;
    Bool        pickup;
    Bool        player;
    Bool        laneSkip;
    Vector2D    position;
    ShapeRect   rect;
    Vector2D    velocity;
    Vector3D    rotation; //(x,y) = rotation center, z = degrees of rotation
    Sprite     *sprite;
    int         health;
    int         maxHealth;
    char       *pickup_type;
    char       *current_pickup;
    int         number_of_collectables;
    Bool        collectableRare;
    float       frame;
    float       frameRate;
    int         frameCount;
    void      (*update)(struct Entity_s *self);
    void      (*think)(struct Entity_s *self);
    void      (*draw)(struct Entity_s *self);
    void      (*free)(struct Entity_s *self);
    void       *data;
}Entity;

/**
 * @brief initialize internal entity management system
 * @param max_entities how many concurrent entities to support
 */
void entity_manager_init(Uint32 max_entities);

/**
 * @brief calls update function on all entities, checks for collisions, applies power-ups and counts collectables
 */
void entity_manager_update_entities();

/**
 * @brief call think function for all entities
 */
void entity_manager_think_entities();


/**
 * @brief call draw on all entities
 */
void entity_manager_draw_entities();


/**
 * @brief free all entities in the system and destroy entity manager
 */
void entity_manager_free();

/**
 * @brief allocate an entity, initialize it to zero and return a pointer to it
 * @return NULL on error (see logs) or a pointer to an initialized entity.
 */
Entity *entity_new();

/**
 * @brief frees provided entity
 * @param ent the entity to free
 */
void entity_free(Entity *ent);

/**
 * @brief draws an entity to the current render frame
 * @param ent the entity to draw
 */
void entity_draw(Entity *ent);


/**
 * @brief fetches the player entity data for use in game.c
 * @return Returns the player entity
 */
Entity fetch_player_data();

#endif
