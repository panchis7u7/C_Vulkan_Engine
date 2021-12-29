#pragma once

#include "core/application.h"

typedef struct game {
    application_config app_config;
    bool8 (*initialize)(struct game* game_instance);
    bool8 (*update)(struct game* game_instance, f32 delta_time);
    bool8 (*render)(struct game* game_instance, f32 delta_time);
    void (*on_resize)(struct game* game_instance, u32 width, u32 height);
    void* state;
} game;
