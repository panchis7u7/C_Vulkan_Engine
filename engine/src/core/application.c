#include "application.h"
#include "game_types.h"
#include "logger.h"
#include "platform/platform.h"
#include "vkcmemory.h"

typedef struct application_state {
    game* game_instance;
    platform_state platform;
    bool8 is_running;
    bool8 is_suspended;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

// Make private for this file. (singleton)
static bool8 initialized = FALSE;
static application_state app_state;

bool8 application_create(game* game_instance) {
    if(initialized) {
        VKCERROR("Application_create called more than once!");
        return FALSE;
    }

    app_state.game_instance = game_instance;

    // Subsystem initialization.
    initialize_logging();

    //TODO: Removal of test cases below.
    VKCFATAL("Test message: %f", 2.71);
    VKCERROR("Test message: %f", 2.71);
    VKCWARN("Test message: %f", 2.71);
    VKCINFO("Test message: %f", 2.71);
    VKCDEBUG("Test message: %f", 2.71);
    VKCTRACE("Test message: %f", 2.71);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if(!platform_startup(
        &app_state.platform, 
        game_instance->app_config.name, 
        game_instance->app_config.start_pos_x, 
        game_instance->app_config.start_pos_y, 
        game_instance->app_config.start_width, 
        game_instance->app_config.start_height)) return FALSE;

    // Initialize the game.
    if(!app_state.game_instance->initialize(app_state.game_instance)) {
        VKCFATAL("Game failed to initialize!");
        return FALSE;
    }

    app_state.game_instance->on_resize(app_state.game_instance, app_state.width, app_state.height);

    initialized = TRUE;
    return TRUE;
}

bool8 application_run() {
    VKCINFO(get_memory_usage_str());
    
    while (app_state.is_running) {
        if(!platform_pump_message(&app_state.platform))
            app_state.is_running = FALSE;

        if(!app_state.is_suspended) {
            if(!app_state.game_instance->update(app_state.game_instance, (f32)0)) {
                VKCFATAL("Game update failed, shutting down...");
                app_state.is_running = FALSE;
                break;
            }

        if(!app_state.game_instance->render(app_state.game_instance, (f32)0)) {
                VKCFATAL("Game render failed, shutting down...");
                app_state.is_running = FALSE;
                break;
            }
        }
    }

    app_state.is_running = FALSE;

    platform_shutdown(&app_state.platform);

    return TRUE;
}