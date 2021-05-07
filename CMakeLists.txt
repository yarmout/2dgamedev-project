cmake_minimum_required(VERSION 3.17)
project(gameframework2d)

set(CMAKE_C_STANDARD 99)

include_directories(gfc/include)
include_directories(gfc/simple_json/include)
include_directories(gfc/simple_logger/include)
include_directories(include)

include_directories(C:/Ubuntu/libs/SDL2-2.0.14/include)
include_directories(C:/Ubuntu/libs/SDL2_ttf-2.0.15/include)
include_directories(C:/Ubuntu/libs/SDL2_mixer-2.0.4/include)
include_directories(C:/Ubuntu/libs/SDL2_image-2.0.5/include)

link_directories(C:/Ubuntu/libs/SDL2-2.0.14/lib/x86)
link_directories(C:/Ubuntu/libs/SDL2_ttf-2.0.15/lib/x86)
link_directories(C:/Ubuntu/libs/SDL2_mixer-2.0.4/lib/x86)
link_directories(C:/Ubuntu/libs/SDL2_image-2.0.5/lib/x86)


add_executable(gameframework2d
        gfc/simple_json/src/simple_json.c
        gfc/simple_json/src/simple_json_array.c
        gfc/simple_json/src/simple_json_error.c
        gfc/simple_json/src/simple_json_list.c
        gfc/simple_json/src/simple_json_object.c
        gfc/simple_json/src/simple_json_parse.c
        gfc/simple_json/src/simple_json_string.c
        gfc/simple_logger/src/simple_logger.c
        gfc/src/gfc_audio.c
        gfc/src/gfc_callbacks.c
        gfc/src/gfc_color.c
        gfc/src/gfc_input.c
        gfc/src/gfc_list.c
        gfc/src/gfc_matrix.c
        gfc/src/gfc_text.c
        gfc/src/gfc_types.c
        gfc/src/gfc_vector.c
        src/game.c
        src/gf2d_draw.c
        src/gf2d_graphics.c
        src/gf2d_sprite.c
        src/camera.c
        src/entity.c
        src/font.c
        src/level.c
        src/shape.c
        src/player.c
        src/obstacle.c
        src/pickup.c
        src/saves.c src/gf2d_windows.c include/gf2d_windows.h src/gf2d_shape.c include/gf2d_shape.h src/gf2d_config.c include/gf2d_config.h src/gf2d_mouse.c include/gf2d_mouse.h src/gf2d_actor.c include/gf2d_actor.h src/gf2d_elements.c include/gf2d_elements.h src/gf2d_element_actor.c include/gf2d_element_actor.h src/gf2d_element_button.c include/gf2d_element_button.h src/gf2d_element_entry.c include/gf2d_element_entry.h src/gf2d_element_label.c include/gf2d_element_label.h src/gf2d_font.c src/gf2d_font.h src/gf2d_element_list.c include/gf2d_element_list.h src/windows_common.c include/windows_common.h)

target_link_libraries(gameframework2d SDL2 SDL2main SDL2_ttf SDL2_mixer SDL2_image)
