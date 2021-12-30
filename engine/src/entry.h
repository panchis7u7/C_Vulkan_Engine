#include "core/application.h"
#include "core/logger.h"
#include "core/vkcmemory.h"
#include "game_types.h"

// Externally defined function to create a game.
extern bool8 create_game(game* out_game);

int main(void) {

    initialize_memory();

    game game_instance;
    if(!create_game(&game_instance)) {
        VKCFATAL("Could not create game!");
        return -1;
    }

    if(!game_instance.render || !game_instance.update || !game_instance.initialize || !game_instance.on_resize) {
        VKCFATAL("The game function pointers must be assigned!");
        return -2;
    }

    if(!application_create(&game_instance)) {
        VKCINFO("The application failed to create!");
        return 1;
    }

    // Begin the game loop.
    if(!application_run()) {
        VKCINFO("The application did not shutdown gracefully!");
        return 2;
    }

    shutdown_memory();

    return 0;
}