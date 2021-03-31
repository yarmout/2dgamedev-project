#include <SDL.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "font.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "level.h"
#include "saves.h"


int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    int i;
    const Uint8 * keys;
    Level *level;
    Font *font;
    TextLine hp_text, powerup_text, collectables_text, rare_collectable_text;
    TextLine previous_game_text, previous_hp_text, previous_collectables_text, previous_rare_collectable_text;
    Entity player;
    int arr[3];
    int* game_data;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    if(load_game(arr))game_data = load_game(arr);
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    camera_set_dimensions(vector2d(1200,720));
    camera_set_position(vector2d(0,0));
    gf2d_sprite_init(1024);
    font_init(10);
    
    entity_manager_init(100);

    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    level = level_load("levels/a_level.json");
    player_spawn(vector2d(0,512));
    font = font_load("fonts/Roboto-MediumItalic.ttf",16);
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        entity_manager_think_entities();
        entity_manager_update_entities();
        player = fetch_player_data();

        level_update(level);
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            level_draw(level);
            
            entity_manager_draw_entities();
            
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);

        //Current game
        gfc_line_sprintf(hp_text, "Current HP: %i", player.HP);
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

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

        if (keys[SDL_SCANCODE_ESCAPE])
        {
            save_game(player);
            done = 1; // exit condition
        }
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
