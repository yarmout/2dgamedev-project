#include <stdlib.h>

#include "simple_json.h"
#include "simple_logger.h"

#include "camera.h"
#include "level.h"
#include "obstacle.h"
#include "pickup.h"


Level *level_new()
{
    Level *level;
    level = (Level *)malloc(sizeof(Level));
    if (!level)
    {
        slog("failed to allocate memory for the game level");
        return NULL;
    }
    memset(level,0,sizeof(Level));
    return level;
}

Level *level_load(const char *filename)
{
    const char *string;
    Level *level;
    SJson *json,*levelJS,*levelMap,*row,*array;
    int rows,columns;
    int count, tileindex;
    int snowManCount;
    float entity_x, entity_y;
    int i,j;
    int tempInt, status;
    float tempFloat;

    if (!filename)
    {
        slog("filename is NULL, cannot load the level");
        return NULL;
    }
    json = sj_load(filename);
    if (!json)return NULL;

    level = level_new();
    if (!level)
    {
        sj_free(json);
        return NULL;
    }
    
    levelJS = sj_object_get_value(json,"level");
    if (!levelJS)
    {
        slog("level json missing level object");
        level_free(level);
        sj_free(json);
        return NULL;
    }
    //Read background image
    array = sj_object_get_value(levelJS,"bgImage");
    count = sj_array_get_count(array);
    level->bgImageCount = count;
    if (count)
    {
        level->bgImage = (Sprite **)gfc_allocate_array(sizeof(Sprite*),count);
        for (i = 0; i < count;i++)
        {
            string = sj_get_string_value(sj_array_get_nth(array,i));
            if (string)
            {
                level->bgImage[i] = gf2d_sprite_load_image((char *)string);
            }
        }
    }

    //Read tileset
    string = sj_get_string_value(sj_object_get_value(levelJS,"tileSet"));
    if (string)
    {
        slog("loading tile set %s",string);
        sj_get_integer_value(sj_object_get_value(levelJS,"tileWidth"),&level->tileWidth);
        sj_get_integer_value(sj_object_get_value(levelJS,"tileHeight"),&level->tileHeight);
        sj_get_integer_value(sj_object_get_value(levelJS,"tileFPL"),&level->tileFPL);
        level->tileSet = gf2d_sprite_load_all(
            (char *)string,
            level->tileWidth,
            level->tileHeight,
            level->tileFPL);
    }
    //Read tile map/level map
    levelMap = sj_object_get_value(levelJS,"tileMap");
    if (!levelMap)
    {
        slog("missing tileMap data");
        level_free(level);
        sj_free(json);
        return NULL;
    }
    rows = sj_array_get_count(levelMap);
    row = sj_array_get_nth(levelMap,0);
    columns = sj_array_get_count(row);
    count = rows * columns;
    level->levelWidth = columns;
    level->levelHeight = rows;
    level->tileMap = (TileTypes*)gfc_allocate_array(sizeof(TileTypes),count);
    if (!level->tileMap)
    {
        level_free(level);
        sj_free(json);
        return NULL;
    }
    level->tileCount = count;
    
    tileindex = 0;
    slog("tilemap data:");
    for (j = 0; j < rows; j++)
    {
        row = sj_array_get_nth(levelMap,j);
        if (!row)continue;// skip it, its bad
        if (columns != sj_array_get_count(row))
        {
            slog("row %i, column count mismatch",j);
            continue;
        }
        for (i = 0; i < columns; i++)
        {
            sj_get_integer_value(sj_array_get_nth(row,i),&tempInt);
            level->tileMap[tileindex] = tempInt;
            printf("%i,",level->tileMap[tileindex++]);
        }
        printf("\n");
    }
    level->levelSize.x = level->levelWidth * level->tileWidth;
    level->levelSize.y = level->levelHeight * level->tileHeight;
    slog("map width: %f, with %i tiles wide, each %i pixels wide", level->levelSize.x, level->levelWidth,level->tileWidth);
    slog("map height: %f, with %i tiles high, each %i pixels tall", level->levelSize.y, level->levelHeight, level->tileHeight);


    //load obstacles
    status = obstacle_load(levelJS, "snow_man");
    if (status == 1)
    {
        sj_free(json);
        return NULL;
    }
    status = obstacle_load(levelJS, "crystal");
    if (status == 1)
    {
        sj_free(json);
        return NULL;
    }
    status = obstacle_load(levelJS, "igloo");
    if (status == 1)
    {
        sj_free(json);
        return NULL;
    }
    status = obstacle_load(levelJS, "stone");
    if (status == 1)
    {
        sj_free(json);
        return NULL;
    }
    status = obstacle_load(levelJS, "trees");
    if (status == 1)
    {
        sj_free(json);
        return NULL;
    }

    //load pickups
    status = pickup_load(levelJS, "invincible");
    if (status == 1)
    {
        sj_free(json);
        return NULL;
    }
    status = pickup_load(levelJS, "extraLife");
    if (status == 1)
    {
        sj_free(json);
        return NULL;
    }
    status = pickup_load(levelJS, "restoreHealth");
    if (status == 1)
    {
        sj_free(json);
        return NULL;
    }
    status = pickup_load(levelJS, "normal");
    if (status == 1)
    {
        sj_free(json);
        return NULL;
    }
    status = pickup_load(levelJS, "rare");
    if (status == 1)
    {
        sj_free(json);
        return NULL;
    }

    sj_free(json);

    return level;
}

int obstacle_load(SJson *levelJS, char *entity_key)
{
    SJson *entityList, *entityMap, *entity;
    int entityCount;
    float entity_x, entity_y, tempFloat;
    float frameWidth, frameHeight;
    char entityPath[100] = "images/obstacles/";
    char fileExtension[5] = ".png";
    strcat(entityPath, entity_key);
    strcat(entityPath, fileExtension);

    entityMap = sj_object_get_value(levelJS, "entityMap");
    if (!entityMap)
    {
        slog("level json missing entityMap object");
        return 1;
    }

    entityList = sj_object_get_value(entityMap, entity_key);
    if (!entityList)
    {
        slog("missing entity list");
        return 1;
    }
    entityCount = sj_array_get_count(entityList);

    for (int k = 0; k < entityCount; k++)
    {
        entity = sj_array_get_nth(entityList, k);
        if (!entity)continue; //skip it, it's bad
        sj_get_float_value(sj_array_get_nth(entity, 0), &tempFloat);
        entity_x = tempFloat;
        sj_get_float_value(sj_array_get_nth(entity, 1), &tempFloat);
        entity_y = tempFloat;


        if (strcmp(entity_key, "snow_man") == 0)
        {
            frameWidth = 128;
            frameHeight = 140;
        }
        if (strcmp(entity_key, "crystal") == 0)
        {
            frameWidth = 97;
            frameHeight = 78;
        }
        if (strcmp(entity_key, "igloo") == 0)
        {
            frameWidth = 356;
            frameHeight = 140;
        }
        if (strcmp(entity_key, "stone") == 0)
        {
            frameWidth = 124;
            frameHeight = 78;
        }
        if (strcmp(entity_key, "trees") == 0)
        {
            frameWidth = 182;
            frameHeight = 140;
        }
        obstacle_spawn(
                vector2d(entity_x, entity_y),
                entityPath,
                frameWidth,
                frameHeight,
                1,
                1,
                1);
    }
    return 0;
}

int pickup_load(SJson *levelJS, char *pickup_key)
{
    SJson *pickupList, *pickupMap, *pickup;
    int pickupCount;
    float pickup_x, pickup_y, tempFloat;
    float frameWidth, frameHeight;
    char pickupPath[100] = "images/pickups/";
    char fileExtension[5] = ".png";
    strcat(pickupPath, pickup_key);
    strcat(pickupPath, fileExtension);

    pickupMap = sj_object_get_value(levelJS, "pickupMap");
    if (!pickupMap)
    {
        slog("level json missing pickupMap object");
        return 1;
    }

    pickupList = sj_object_get_value(pickupMap, pickup_key);
    if (!pickupList)
    {
        slog("missing pickup list");
        return 1;
    }
    pickupCount = sj_array_get_count(pickupList);

    for (int k = 0; k < pickupCount; k++)
    {
        pickup = sj_array_get_nth(pickupList, k);
        if (!pickup)
        {
            slog("Bad pickup detected!");
        }
        sj_get_float_value(sj_array_get_nth(pickup, 0), &tempFloat);
        pickup_x = tempFloat;
        sj_get_float_value(sj_array_get_nth(pickup, 1), &tempFloat);
        pickup_y = tempFloat;


        if (strcmp(pickup_key, "invincible") == 0)
        {
            frameWidth = 70;
            frameHeight = 70;
        }
        if (strcmp(pickup_key, "extraLife") == 0)
        {
            frameWidth = 70;
            frameHeight = 70;
        }
        if (strcmp(pickup_key, "restoreHealth") == 0)
        {
            frameWidth = 70;
            frameHeight = 70;
        }
        if (strcmp(pickup_key, "normal") == 0)
        {
            frameWidth = 60;
            frameHeight = 45;
        }
        if (strcmp(pickup_key, "rare") == 0)
        {
            frameWidth = 60;
            frameHeight = 45;
        }
        pickup_spawn(
                vector2d(pickup_x, pickup_y),
                pickupPath,
                pickup_key,
                frameWidth,
                frameHeight,
                1,
                1,
                1);
    }
    return 0;
}


void level_update(Level *level)
{
    SDL_Rect camera;
    if (!level)return;
    camera = camera_get_rect();
    //snap camera to the level bounds
    if ((camera.x + camera.w) > (int)level->levelSize.x)
    {
        camera.x = level->levelSize.x - camera.w;
    }
    if ((camera.y + camera.h) > (int)level->levelSize.y)
    {
        camera.y = level->levelSize.y - camera.h;
    }
    if (camera.x < 0)camera.x = 0;
    if (camera.y < 0)camera.y = 0;
    camera_set_position(vector2d(camera.x,camera.y));
}

void level_free(Level *level)
{
    int i;
    if (!level)return;// nothing to do
    
    if (level->tileMap != NULL)
    {
        free(level->tileSet);
        level->tileMap = NULL;
    }
    if (level->bgImageCount)
    {
        for (i = 0; i < level->bgImageCount;i++)
        {
            gf2d_sprite_free(level->bgImage[i]);
        }
        free(level->bgImage);
    }
    gf2d_sprite_free(level->tileSet);
    
    free(level);
}

void level_draw(Level *level)
{
    SDL_Rect camera;
    Vector2D offset,drawPosition,parallax;
    int i;
    if (!level)
    {
        slog("cannot draw level, NULL pointer provided");
        return;
    }
    // draw the background first
    offset = camera_get_offset();
    if (level->bgImageCount)
    {
        camera = camera_get_rect();
        for (i = 0; i < level->bgImageCount;i++)
        {
            parallax.x = (float)(level->bgImage[i]->frame_w - camera.w)/ (level->levelSize.x - camera.w);
            parallax.y = (float)(level->bgImage[i]->frame_h - camera.h)/ (level->levelSize.y - camera.h);
            
            gf2d_sprite_draw_image(level->bgImage[i],vector2d(offset.x * parallax.x,offset.y * parallax.y));        
        }
            
    }
    //then draw the tiles
    
    if (!level->tileMap)
    {
        slog("not tiles loaded for the level, cannot draw it");
        return;
    }
    for (i = 0; i < level->tileCount; i++)
    {
        if (level->tileMap[i] == 0)continue;
        drawPosition.x = ((i % level->levelWidth)*level->tileSet->frame_w);
        drawPosition.y = ((i / level->levelWidth)*level->tileSet->frame_h);
        if (!camera_rect_on_screen(gfc_sdl_rect(drawPosition.x,drawPosition.y,level->tileSet->frame_w,level->tileSet->frame_h)))
        {
            //tile is off camera, skip
            continue;
        }
        drawPosition.x += offset.x;
        drawPosition.y += offset.y;
        gf2d_sprite_draw(
            level->tileSet,
            drawPosition,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            level->tileMap[i] - 1);
    }
}



/*file footer*/
