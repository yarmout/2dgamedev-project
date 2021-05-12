#include <stdlib.h>
#include "simple_logger.h"

#include "camera.h"
#include "entity.h"
#include "shape.h"
#include "gfc_audio.h"



typedef struct
{
    Entity *entity_list;
    Uint32  max_entities;
}EntityManager;

static EntityManager entity_manager = {0};

void entity_manager_init(Uint32 max_entities)
{
    if (max_entities == 0)
    {
        slog("cannot allocate 0 entities!");
        return;
    }
    if (entity_manager.entity_list != NULL)
    {
        entity_manager_free();
    }
    entity_manager.entity_list = (Entity *)gfc_allocate_array(sizeof (Entity),max_entities);
    if (entity_manager.entity_list == NULL)
    {
        slog("failed to allocate entity list!");
        return;
    }
    entity_manager.max_entities = max_entities;
    atexit(entity_manager_free);
    slog("entity system initialized");
}

void entity_manager_free()
{
    if (entity_manager.entity_list != NULL)
    {
        free(entity_manager.entity_list);
    }
    memset(&entity_manager,0,sizeof(EntityManager));
    slog("entity system closed");
}

void entity_update(Entity *self)
{
    if (!self)return;
    // DO ANY GENERIC UPDATE CODE
    vector2d_add(self->position,self->position,self->velocity);
    self->frame += self->frameRate;
    if (self->frame >= self->frameCount)self->frame = 0;
    // IF THERE IS A CUSTOM UPDATE, DO THAT NOW
    if (self->update)self->update(self);
}

void entity_manager_update_entities()
{
    int i, j;
    int playerIndex = {0};

    if (entity_manager.entity_list == NULL)
    {
        slog("entity system does not exist");
        return;
    }
    //Finds which index is the player entity
    for (j = 0; j < entity_manager.max_entities; j++)
    {
        if (entity_manager.entity_list[j].player == true)
        {
            playerIndex = j;
        }
    }

    for (i = 0; i < entity_manager.max_entities; i++)
    {
        if (entity_manager.entity_list[i]._inuse == 0)continue;
        entity_update(&entity_manager.entity_list[i]);

        //Handles player collisions with entities
        if(shape_rect_collision(entity_manager.entity_list[playerIndex].rect, entity_manager.entity_list[i].rect) && i != playerIndex)
        {
            //Handles pickups and power-ups
            if (entity_manager.entity_list[i].pickup == true)
            {
                entity_manager.entity_list[i].rect.x = 5000;
                entity_manager.entity_list[i].rect.y = 5000;
                if (strcmp(entity_manager.entity_list[i].pickup_type, "invincible") == 0)
                {
                    entity_manager.entity_list[playerIndex].current_pickup = "invincible";
                }
                if (strcmp(entity_manager.entity_list[i].pickup_type, "extraLife") == 0)
                {

                    entity_manager.entity_list[playerIndex].health += 1;
                    entity_manager.entity_list[playerIndex].maxHealth += 1;
                }
                if (strcmp(entity_manager.entity_list[i].pickup_type, "restoreHealth") == 0)
                {
                    entity_manager.entity_list[playerIndex].health = entity_manager.entity_list[playerIndex].maxHealth;
                }
                if (strcmp(entity_manager.entity_list[i].pickup_type, "normal") == 0)
                {
                    entity_manager.entity_list[playerIndex].number_of_collectables += 1;
                }
                if (strcmp(entity_manager.entity_list[i].pickup_type, "rare") == 0)
                {
                    entity_manager.entity_list[playerIndex].collectableRare = true;
                }
            }

            //Skips collision damage if invincible
            else if (strcmp(entity_manager.entity_list[playerIndex].current_pickup, "invincible") == 0)continue;

            //Take damage upon collision
            else
            {
                entity_manager.entity_list[i].rect.x = 5000;
                entity_manager.entity_list[i].rect.y = 5000;
                entity_manager.entity_list[playerIndex].health -= 1;

                Sound *damage_sound = gfc_sound_load("music/damage_sound.ogg", 50, 2);
                gfc_sound_play(damage_sound, 1, 50, 2, 1);
                gfc_sound_free(damage_sound);
            }
        }
    }
}

void entity_manager_think_entities()
{
    int i;
    if (entity_manager.entity_list == NULL)
    {
        slog("entity system does not exist");
        return;
    }
    for (i = 0; i < entity_manager.max_entities; i++)
    {
        if (entity_manager.entity_list[i]._inuse == 0)continue;
        if (entity_manager.entity_list[i].think != NULL)
        {
            entity_manager.entity_list[i].think(&entity_manager.entity_list[i]);
        }
    }
}

void entity_manager_draw_entities()
{
    int i;
    if (entity_manager.entity_list == NULL)
    {
        slog("entity system does not exist");
        return;
    }
    for (i = 0; i < entity_manager.max_entities; i++)
    {
        if (entity_manager.entity_list[i]._inuse == 0)continue;
        entity_draw(&entity_manager.entity_list[i]);
    }
}


Entity *entity_new()
{
    int i;
    if (entity_manager.entity_list == NULL)
    {
        slog("entity system does not exist");
        return NULL;
    }
    for (i = 0;i < entity_manager.max_entities;i++)
    {
        if (entity_manager.entity_list[i]._inuse)continue;// someone else is using this one
        memset(&entity_manager.entity_list[i],0,sizeof(Entity));
        entity_manager.entity_list[i]._inuse = 1;
        return &entity_manager.entity_list[i];
    }
    slog("no free entities available");
    return NULL;
}

void entity_free(Entity *ent)
{
    if (!ent)
    {
        slog("cannot free a NULL entity");
        return;
    }
    gf2d_sprite_free(ent->sprite);
    ent->sprite = NULL;
    ent->_inuse = 0;
}

void entity_draw(Entity *ent)
{
    Vector2D drawPosition,offset;
    if (!ent)
    {
        slog("cannot draw a NULL entity");
        return;
    }
    if (ent->draw)
    {
        ent->draw(ent);
    }
    else
    {
        if (ent->sprite == NULL)
        {
            return;// nothing to draw
        }
        offset = camera_get_offset();
        if (!camera_rect_on_screen(gfc_sdl_rect(ent->position.x,ent->position.y,ent->sprite->frame_w,ent->sprite->frame_h)))
        {
            //entity is off camera, skip
            return;
        }
        drawPosition.x = ent->position.x + offset.x;
        drawPosition.y = ent->position.y + offset.y;

        gf2d_sprite_draw(
            ent->sprite,
            drawPosition,
            NULL,
            NULL,
            &ent->rotation,
            NULL,
            NULL,
            (Uint32)ent->frame);
    }
}

Entity fetch_player_data()
{
    int j;
    int playerIndex = {0};
    Entity placeholderEnt = {0};

    if (entity_manager.entity_list == NULL)
    {
        slog("entity system does not exist");
        return placeholderEnt;
    }
    //Finds which index is the player entity
    for (j = 0; j < entity_manager.max_entities; j++)
    {
        if (entity_manager.entity_list[j].player == true)
        {
            playerIndex = j;
        }
    }
    return entity_manager.entity_list[playerIndex];
}


/*eol@eof*/
