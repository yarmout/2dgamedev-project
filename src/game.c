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

static int _done = 0;
static Window *_quit = NULL;
Level *level;

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
    player_spawn(vector2d(0,1000));
}
void levelOne(void *data)
{
    level = level_load("levels/a_level.json");
    player_spawn(vector2d(0,512));
}
void levelTwo(void *data)
{
    level = level_load("levels/b_level.json");
    player_spawn(vector2d(0,512));
}
void levelThree(void *data)
{
    level = level_load("levels/c_level.json");
    player_spawn(vector2d(0,512));
}
void onPlay(void *data)
{
    window_level_selector("Select a Level",levelOne,levelTwo,levelThree,NULL,NULL,NULL);
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
    gf2d_graphics_set_frame_delay(16);
    gfc_audio_init(256,16,4,1,1,1);
    gf2d_sprite_init(1024);
    gf2d_action_list_init(128);
    gf2d_font_init("config/font.cfg");
    gfc_input_init("config/input.cfg");
    gf2d_windows_init(128);
    entity_manager_init(100);

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
    music = Mix_LoadMUS("music/wisdom.mp3");
    Mix_PlayMusic(music, -1);
    window_main_menu("Penguin Sledding Adventure",onPlay,onExit,NULL,NULL);

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

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen between clear_screen and next_frame
        //backgrounds drawn first
        level_draw(level);

        entity_manager_draw_entities();

        //UI elements last
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

