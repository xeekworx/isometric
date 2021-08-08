#include "application.h"
#include "main_module.h"
#include <algorithm>

using namespace isometric;
using namespace isometric::game;

void main_module::on_registered()
{
    grasslands_image = image::load("grasslands", "content/grassland_tiles.png");

    constexpr unsigned tile_width = 64;
    constexpr unsigned tile_height = 32;

    main_camera = isometric::camera::create(
        0, 0, // viewport x, viewport y
        application::get_app()->get_setup().screen_width, 
        application::get_app()->get_setup().screen_height,
        0, 0 // current tile position x, y
    
    );

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

    world = std::make_unique<isometric::world>(
        map, 
        main_camera
    );
}

void main_module::on_unregister()
{

}

void main_module::on_update(double delta_time)
{
    constexpr double speed = 50.0; // Tiles per second of movement
    auto input = application::get_app()->get_input();

    if (input->scancode_down(SDL_SCANCODE_LEFT)) {
        main_camera->set_current_x(std::max(main_camera->get_current_x() - static_cast<float>(speed * delta_time), 0.0f));
        //main_camera->set_current_x(main_camera->get_current_x() - (speed * delta_time));
    }
    if (input->scancode_down(SDL_SCANCODE_RIGHT)) {
        main_camera->set_current_x(std::min(main_camera->get_current_x() + static_cast<float>(speed * delta_time), static_cast<float>((int)map->get_map_width() - (int)world->get_max_horizontal_tiles() - 1)));
        //main_camera->set_current_x(main_camera->get_current_x() + (speed * delta_time));
    }
    if (input->scancode_down(SDL_SCANCODE_UP)) {
        main_camera->set_current_y(std::max(main_camera->get_current_y() - static_cast<float>((speed * 2) * delta_time), 0.0f));
        //main_camera->set_current_y(main_camera->get_current_y() - ((speed * 2) * delta_time));
    }
    if (input->scancode_down(SDL_SCANCODE_DOWN)) {
        main_camera->set_current_y(std::min(main_camera->get_current_y() + static_cast<float>((speed * 2) * delta_time), static_cast<float>((int)map->get_map_height() - (int)world->get_max_vertical_tiles() - 1)));
        //main_camera->set_current_y(main_camera->get_current_y() + ((speed * 2) * delta_time));
    }

    auto renderer = application::get_app()->get_graphics()->get_renderer();

    world->update();
    world->render(renderer);
}

void main_module::on_late_update(double delta_time)
{

}

void main_module::on_fixed_update(double fixed_delta_time)
{

}