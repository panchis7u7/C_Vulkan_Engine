#include "game.h"
#include <core/logger.h>

bool8 game_initialize(game* game_instance) {
    (void)game_instance;
    VKCDEBUG("game_initialized() called!");
    return TRUE;
}

bool8 game_update(game* game_instance, f32 delta_time) {
    (void)game_instance;
    (void)delta_time;
    return TRUE;
}

bool8 game_render(game* game_instance, f32 delta_time) {
    (void)game_instance;
    (void)delta_time;
    return TRUE;
}

void game_on_resize(game* game_instance, u32 width, u32 height) {
    (void)game_instance;
    (void)width;
    (void)height;
}