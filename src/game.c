#include <SDL.h>
#include <stdio.h>

#include <simple_logger.h>
#include "gfc_input.h"

#include "saves.h"
#include "gf2d_actor.h"
#include "entity.h"
#include "level.h"
#include "player.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_windows.h"
#include "gf2d_font.h"
#include "gf2d_mouse.h"
#include "gf2d_draw.h"
#include "camera.h"
#include "windows_common.h"
#include "gf2d_space.h"
#include "gfc_audio.h"
#include "font.h"

static int _done = 0;
static Window *_quit = NULL;
Level *level;

Bool infinite_level = false;

void penguinZero(void *data)
{
    player_spawn(vector2d(0,512), 0);
}
void penguinOne(void *data)
{
    player_spawn(vector2d(0,512), 1);
}
void penguinTwo(void *data)
{
    player_spawn(vector2d(0,512), 2);
}
void penguinThree(void *data)
{
    player_spawn(vector2d(0,512), 3);
}
void onCancel(void *data)
{
    _quit = NULL;
}
void onExit(void *data)
{
    _done = 1;
    _quit = NULL;
}
void mainMenu(void *data)
{
    level = level_load("levels/main_menu.json");
}
void levelOne_s(void *data)
{
    window_penguin_selector("Penguin Selector", penguinZero, penguinOne, penguinTwo, penguinThree, NULL,NULL,NULL,NULL);
    level = level_load("levels/a_level.json");
    Mix_Music *music;
    music = Mix_LoadMUS("music/in_game_music.ogg");
    Mix_PlayMusic(music, -1);
}
void levelTwo_s(void *data)
{
    window_penguin_selector("Penguin Selector", penguinZero, penguinOne, penguinTwo, penguinThree, NULL,NULL,NULL,NULL);
    level = level_load("levels/b_level.json");
    Mix_Music *music;
    music = Mix_LoadMUS("music/in_game_music.ogg");
    Mix_PlayMusic(music, -1);
}
void levelThree_s(void *data)
{
    window_penguin_selector("Penguin Selector", penguinZero, penguinOne, penguinTwo, penguinThree, NULL,NULL,NULL,NULL);
    level = level_load("levels/c_level.json");
    Mix_Music *music;
    music = Mix_LoadMUS("music/in_game_music.ogg");
    Mix_PlayMusic(music, -1);
    infinite_level = true;
}
void levelFour_s(void *data)
{
    window_penguin_selector("Penguin Selector", penguinZero, penguinOne, penguinTwo, penguinThree, NULL,NULL,NULL,NULL);
    level = level_load("levels/d_level.json");
    Mix_Music *music;
    music = Mix_LoadMUS("music/in_game_music.ogg");
    Mix_PlayMusic(music, -1);
}
void levelFive_s(void *data)
{
    window_penguin_selector("Penguin Selector", penguinZero, penguinOne, penguinTwo, penguinThree, NULL,NULL,NULL,NULL);
    level = level_load("levels/e_level.json");
    Mix_Music *music;
    music = Mix_LoadMUS("music/in_game_music.ogg");
    Mix_PlayMusic(music, -1);
    infinite_level = true;
}
void onPlaySecret(void *data)
{
    window_level_selector("Select a Level", levelOne_s, levelTwo_s, levelThree_s,levelFour_s,levelFive_s,NULL,NULL,NULL,NULL,NULL,1);
}
void levelOne(void *data)
{
    level = level_load("levels/a_level.json");
    player_spawn(vector2d(0,512), 0);
    Mix_Music *music;
    music = Mix_LoadMUS("music/in_game_music.ogg");
    Mix_PlayMusic(music, -1);
}
void levelTwo(void *data)
{
    level = level_load("levels/b_level.json");
    player_spawn(vector2d(0,512), 0);
    Mix_Music *music;
    music = Mix_LoadMUS("music/in_game_music.ogg");
    Mix_PlayMusic(music, -1);
}
void levelThree(void *data)
{
    level = level_load("levels/c_level.json");
    player_spawn(vector2d(0,512), 0);
    Mix_Music *music;
    music = Mix_LoadMUS("music/in_game_music.ogg");
    Mix_PlayMusic(music, -1);
}
void onPlay(void *data)
{
    window_level_selector("Select a Level", levelOne, levelTwo, levelThree,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0);
}

int main(int argc, char * argv[])
{
    /*variable declarations*/
    const Uint8 * keys;
    int mx,my;
    float mf;
    Entity player;
    Space *space = NULL;
    Mix_Music *music;
    Font_old *font;
    TextLine hp_text, powerup_text, collectables_text, rare_collectable_text;
    TextLine previous_game_text, previous_hp_text, previous_collectables_text, previous_rare_collectable_text;
    int arr[3];
    int* game_data = {0};

    /*game initialization*/

    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
            "gf2d",
            1200,
            720,
            1200,
            720,
            vector4d(0,0,0,255),
            0,
            0);
    if(load_game(arr))game_data = load_game(arr);
    font_init(10);
    font = font_load("fonts/Roboto-MediumItalic.ttf",16);
    gf2d_graphics_set_frame_delay(16);
    gfc_audio_init(256,16,4,1,1,1);
    gf2d_sprite_init(1024);
    gf2d_action_list_init(128);
    gf2d_font_init("config/font.cfg");
    gfc_input_init("config/input.cfg");
    gf2d_windows_init(128);
    entity_manager_init(1024);

    camera_set_dimensions(vector2d(1200,720));
    camera_set_position(vector2d(0,0));

    SDL_ShowCursor(SDL_DISABLE);

    /*game setup*/
    gf2d_mouse_load("actors/mouse.actor");
    space = gf2d_space_new_full(
            3,
            gf2d_rect(0,0,1200,700),
            0.1,
            vector2d(0,0.1),
            1,
            1);
    mf = 0;
    mainMenu(NULL);
//  levelOne_s(NULL);
    music = Mix_LoadMUS("music/main_menu_music.mp3");
    Mix_PlayMusic(music, -1);
    if (game_data[2] == 1)window_main_menu("Penguin Sledding Adventure", onPlaySecret, onExit, NULL, NULL);
    else window_main_menu("Penguin Sledding Adventure", onPlay, onExit, NULL, NULL);

    /*main game loop*/
    while(!_done)
    {
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL);

        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        gf2d_windows_update_all();
        entity_manager_think_entities();
        entity_manager_update_entities();
        gf2d_mouse_update();
        gf2d_space_update(space);
        player = fetch_player_data();
        level_update(level);

        if (infinite_level == true && player.collectableRare == true)
        {
            level = level_load("levels/e_level.json");
        }

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen between clear_screen and next_frame
        //backgrounds drawn first
        level_draw(level);

        entity_manager_draw_entities();

        //UI elements last

        //Save Data

        //Current game
        gfc_line_sprintf(hp_text, "Current HP: %i", player.health);
        gfc_line_sprintf(powerup_text, "Current power-up: %s", player.current_pickup);
        gfc_line_sprintf(collectables_text, "# of fish collected: %i", player.number_of_collectables);
        if (player.collectableRare)gfc_line_sprintf(rare_collectable_text, "Rare fish found!", player.collectableRare);
        else gfc_line_sprintf(rare_collectable_text, "Rare fish not yet found!", player.collectableRare);

        //Previous game
        if (game_data)
        {
            gfc_line_sprintf(previous_game_text, "Previous game score:");
            gfc_line_sprintf(previous_hp_text, "Current HP: %i", game_data[1]);
            gfc_line_sprintf(previous_collectables_text, "# of fish collected: %i", game_data[0]);
            if (game_data[2] == 1)gfc_line_sprintf(previous_rare_collectable_text, "Rare fish found!", player.collectableRare);
            else gfc_line_sprintf(previous_rare_collectable_text, "Rare fish not yet found!", player.collectableRare);

            font_render(font, previous_game_text, vector2d(1000, 0), gfc_color8(255, 0, 0, 255));
            font_render(font, previous_hp_text, vector2d(1000, 16), gfc_color8(255, 0, 0, 255));
            font_render(font, previous_collectables_text, vector2d(1000, 32), gfc_color8(255, 0, 0, 255));
            font_render(font, previous_rare_collectable_text, vector2d(1000, 48), gfc_color8(255, 0, 0, 255));
        }

        //Current game
        font_render(font, hp_text, vector2d(32, 0), gfc_color8(255, 0, 0, 255));
        font_render(font, powerup_text, vector2d(32, 16), gfc_color8(255, 0, 0, 255));
        font_render(font, collectables_text, vector2d(32, 32), gfc_color8(255, 0, 0, 255));
        font_render(font, rare_collectable_text, vector2d(32, 48), gfc_color8(255, 0, 0, 255));

        gf2d_windows_draw_all();
        gf2d_mouse_draw();

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

        if (keys[SDL_SCANCODE_ESCAPE] && _quit == NULL)
        {
            save_game(player);
            _quit = window_yes_no("Exit?",onExit,onCancel,NULL,NULL);
        }

//        if (keys[SDL_SCANCODE_ESCAPE])
//        {
//            save_game(player);
//            _done = 1; // exit condition
//        }
    }
    slog("---==== END ====---");
    return 0;
}

