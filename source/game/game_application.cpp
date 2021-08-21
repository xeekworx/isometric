#include "game_application.h"

using namespace isometric::game;
using namespace isometric::assets;

bool game_application::on_start()
{
    main_camera = isometric::camera::create(
        0, 0, // viewport x, viewport y
        application::get_app()->get_setup().screen_width,
        application::get_app()->get_setup().screen_height,
        0, 0 // current tile position x, y
    );

    if (!load_map()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load map");
        return false;
    }
    else {
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Loaded map");
    }

    world = std::make_shared<isometric::world>(
        map,
        main_camera
    );

    this->camera_module = module::create<isometric::game::camera_module>(true); 
    this->camera_module->setup(map, world);
    register_module(this->camera_module);

    this->player_module = module::create<isometric::game::player_module>(true);
    this->player_module->setup(this->map, this->world);
    this->player_module->set_player_location(SDL_FPoint{ 100.0f, 100.0f });
    register_module(this->player_module);

    this->fps_display_module = module::create<isometric::game::fps_display_module>(true);
    register_module(this->fps_display_module);

    return application::on_start();
}

bool isometric::game::game_application::load_map()
{
    constexpr unsigned tile_width = 64;
    constexpr unsigned tile_height = 32;

    grasslands_image = image::load("grasslands", "content/grassland_tiles.png");

    map = isometric::tile_map::create(
        1024,           // entire map width in tiles
        1024,           // entire map height in tiles
        tile_width,
        tile_height
    );

    map->add_image(
        isometric::tile_image::create(
            "selection", 0,
            grasslands_image->get_texture(),
            960, 160,
            tile_width, tile_height
        )
    );
    map->set_selection_image(0);

    map->add_layer("grass");
    unsigned grass_source_x = 0;
    for (unsigned i = 1, grass_source_x = 0; i < 16; i++, grass_source_x += 64) {

        auto grass_tile_image = isometric::tile_image::create(
            "grass" + std::to_string(i), i,
            grasslands_image->get_texture(),
            grass_source_x, 0,
            tile_width, tile_height
        );

        map->add_image(grass_tile_image);
        map->add_layer_default_image("grass", i);
    }

    unsigned foliage_layer_id = map->add_layer("foliage");
    auto bush1_tile_image = isometric::tile_image::create(
        "bush1", 99,
        grasslands_image->get_texture(),
        512, 320,
        tile_width, tile_height * 2
    );

    map->add_image(bush1_tile_image);

    map->set_tile(0, 0, isometric::tile())->set_image_id(foliage_layer_id, 99);
    map->set_tile(9, 9, isometric::tile())->set_image_id(foliage_layer_id, 99);

    return true;
}

void isometric::game::game_application::on_update(double delta_time)
{
    auto renderer = application::get_app()->get_graphics()->get_renderer();

    world->update(delta_time);
    world->render(renderer, delta_time);

    return application::on_update(delta_time);
}

void isometric::game::game_application::on_fixed_update(double fixed_delta_time)
{

}
