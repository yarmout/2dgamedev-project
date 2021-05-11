#include "simple_logger.h"

#include "pickup.h"
#include "camera.h"


void pickup_update(Entity *self);

Entity *pickup_spawn(Vector2D position, char *sprite_filename, char *pick_up_type, float frameWidth, float frameHeight, int framesPerLine, float frameRate, int frameCount)
{
    Entity *ent;
    ent = entity_new();
    if (!ent)
    {
        slog("failed to create entity for the pickup_type");
        return NULL;
    }
    ent->sprite = gf2d_sprite_load_all(sprite_filename,frameWidth,frameHeight,framesPerLine,0);
    vector2d_copy(ent->position,position);
    ent->rect.x = position.x;
    ent->rect.y = position.y;
    ent->rect.h = frameHeight;
    ent->rect.w = frameWidth;
    ent->frameRate = frameRate;
    ent->frameCount = frameCount;
    ent->update = pickup_update;
    ent->pickup = true;
    ent->pickup_type = pick_up_type;
    return ent;
}


void pickup_update(Entity *self)
{
    Vector2D camera;
    Vector2D cameraSize;

    if (!self)return;
    cameraSize = camera_get_dimensions();
    camera.x = (self->position.x + 64) - (cameraSize.x * 0.5);
    camera.y = (self->position.y + 64) - (cameraSize.y * 0.5);
    camera_set_position(camera);
}

/**/