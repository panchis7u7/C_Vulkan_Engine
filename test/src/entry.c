#include <entry.h>>

bool8 create_game(game* out_game) {
    out_game->app_config.name = "Vulkan game engine";
    out_game->app_config.start_pos_x = 100;
    out_game->app_config.start_pos_y = 100;
    out_game->app_config.start_width = 1200;
    out_game->app_config.start_height = 720; 
}