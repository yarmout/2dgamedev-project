

#include "simple_logger.h"

#include "player.h"
#include "camera.h"


void player_update(Entity *self);
void player_think(Entity *self);

Entity *player_spawn(Vector2D position, int type)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the player");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all("images/lr_penguin2_128x128x8.png",128,128,8,0);
    vector2d_copy(ent->position,position);
    ent->rect.h = 128;
    ent->rect.w = 128;
    ent->frameRate = 0.1;
    ent->frameCount = 8;
    ent->update = player_update;
    ent->think = player_think;
    ent->rotation.x = 64;
    ent->rotation.y = 64;
    ent->player = true;
    ent->pickup = false;
    ent->current_pickup = "";
    ent->number_of_collectables = 0;
    ent->health = 3;
    ent->maxHealth = 3;
    ent->laneSkip = false;

    if (type == 1)
    {
        ent->health = 5;
    }
    if (type == 2)
    {
        ent->laneSkip = true;
    }
    if (type == 3)
    {
        ent->maxHealth = 4;
    }

    return ent;
}


void player_update(Entity *self)
{
    Vector2D camera;
    Vector2D cameraSize;

    if (!self)return;
    cameraSize = camera_get_dimensions();
    camera.x = (self->position.x + 64) - (cameraSize.x * 0.5);
    camera.y = (self->position.y + 64) - (cameraSize.y * 0.5);
    camera_set_position(camera);
    // apply dampening on velocity
    vector2d_scale(self->velocity,self->velocity,0.75);
    if (vector2d_magnitude_squared(self->velocity) < 2)
    {
        vector2d_clear(self->velocity);
    }
    self->rect.x = self->position.x;
    self->rect.y = self->position.y;
}

void player_think(Entity *self)
{
    const Uint8 *keys;
    Vector2D aimdir, thrust;
    float topLane, middleLane, bottomLane;
    int factor;
    if (!self)return;

    //Read keyboard input
    keys = SDL_GetKeyboardState(NULL);

    //Ensure tile positions line up
    topLane = 332; // Halfway between middle of screen and top of screen
    middleLane = 512; // Middle of screen
    bottomLane = 692; // Halfway between middle of screen and bottom of screen

    //Calculate movement
    aimdir.x = 1;
    aimdir.y = 0;

    //Speed up or slow down
    factor = 1;
    if (keys[SDL_SCANCODE_W])factor = 5;
    if (keys[SDL_SCANCODE_S])factor -= 1; // factor = 1 doesn't work... This lets the player stop moving, that's a bug

    //Apply movement
    vector2d_normalize (&aimdir);
    vector2d_scale(thrust, aimdir, factor);
    vector2d_add(self->velocity,self->velocity,thrust);

    //Lane switching
    // Could add a smoother transition, instead of snapping into the lane
    //Polished movement vv
    /*
    if(keys[SDL_SCANCODE_UP])
    {
        if (self->position.y == bottomLane)self->position.y = middleLane;
        else if (self->position.y == middleLane)self->position.y = topLane;
    }
    if(keys[SDL_SCANCODE_DOWN])
    {
        if (self->position.y == middleLane)self->position.y = bottomLane;
        else if (self->position.y == topLane)self->position.y = middleLane;
    }
     */

    extern Bool infinite_level;
    if (self->collectableRare == true && infinite_level == true)
    {
        self->position.x = 0;
        self->position.y = 512;
        self->collectableRare = false;
    }

    if (self->laneSkip == true)
    {
        if (keys[SDL_SCANCODE_1])self->position.y = topLane;
        if (keys[SDL_SCANCODE_2])self->position.y = middleLane;
        if (keys[SDL_SCANCODE_3])self->position.y = bottomLane;
    }
    else if (self->laneSkip == false)
    {
        if (keys[SDL_SCANCODE_1] && self->position.y == middleLane)self->position.y = topLane;
        if (keys[SDL_SCANCODE_2])self->position.y = middleLane;
        if (keys[SDL_SCANCODE_3] && self->position.y == middleLane)self->position.y = bottomLane;
    }
}

/**/