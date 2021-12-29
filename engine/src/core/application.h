#pragma once

#include "defines.h"
#include "game_types.h"

typedef struct application_config {
    i16 start_pos_x;    // Window starting position in the x axis.
    i16 start_pos_y;    // Window starting position in the y axis.
    i16 start_width;    // Window starting width.
    i16 start_height;   // Window starting height.
    char* name;         // Application name used in window title.
} application_config;

VKCAPI bool8 application_create(game* game_instance);
VKCAPI bool8 application_run();