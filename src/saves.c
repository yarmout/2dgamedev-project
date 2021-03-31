#include "saves.h"
#include "simple_logger.h"

void save_game(Entity player)
{
    FILE * file_pointer;

    file_pointer = fopen ("saves/lastgame.save", "w");
    fprintf(file_pointer, "{ \"fishCollected\": %i, \"HP\": %i, \"rareFish\": %i}", player.number_of_collectables, player.HP, player.collectableRare);

    fclose(file_pointer);
}

int *load_game(int *game_data)
{
    SJson *save, *fishCollectedJson, *HPJson, *rareFishJson;
    int fishCollected, HP, rareFish;
    const char *filename;

    filename = "saves/lastgame.save";
    save = sj_load(filename);
    if (!save)return NULL;

    fishCollectedJson = sj_object_get_value(save, "fishCollected");
    if (!fishCollectedJson)
    {
        slog("save missing fishCollected object");
        sj_free(save);
        return NULL;
    }
    sj_get_integer_value(fishCollectedJson, &fishCollected);
    game_data[0] = fishCollected;

    HPJson = sj_object_get_value(save, "HP");
    if (!HPJson)
    {
        slog("save missing HP object");
        sj_free(save);
        return NULL;
    }
    sj_get_integer_value(HPJson, &HP);
    game_data[1] = HP;

    rareFishJson = sj_object_get_value(save, "rareFish");
    if (!rareFishJson)
    {
        slog("save missing rareFish object");
        sj_free(save);
        return NULL;
    }
    sj_get_integer_value(rareFishJson, &rareFish);
    game_data[2] = rareFish;
    return game_data;
}