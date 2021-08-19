#include "application.h"
#include "camera_module.h"
#include <algorithm>

using namespace isometric;
using namespace isometric::game;

void camera_module::setup(std::shared_ptr<tile_map> map, std::shared_ptr<isometric::world> world)
{
    this->map = map;
    this->world = world;
}

void camera_module::on_registered()
{

}

void camera_module::on_unregister()
{

}

void camera_module::on_update(double delta_time)
{
    constexpr double speed = 50.0; // Tiles per second of movement
    auto main_camera = world->get_main_camera();

    if (!main_camera) return;

    if (input::scancode_down(SDL_SCANCODE_LEFT)) {
        main_camera->set_current_x(std::max(main_camera->get_current_x() - static_cast<float>(speed * delta_time), 0.0f));
        //main_camera->set_current_x(main_camera->get_current_x() - (speed * delta_time));
    }
    if (input::scancode_down(SDL_SCANCODE_RIGHT)) {
        main_camera->set_current_x(std::min(main_camera->get_current_x() + static_cast<float>(speed * delta_time), static_cast<float>((int)map->get_map_width() - (int)world->get_max_horizontal_tiles() - 1)));
        //main_camera->set_current_x(main_camera->get_current_x() + (speed * delta_time));
    }
    if (input::scancode_down(SDL_SCANCODE_UP)) {
        main_camera->set_current_y(std::max(main_camera->get_current_y() - static_cast<float>((speed * 2) * delta_time), 0.0f));
        //main_camera->set_current_y(main_camera->get_current_y() - ((speed * 2) * delta_time));
    }
    if (input::scancode_down(SDL_SCANCODE_DOWN)) {
        main_camera->set_current_y(std::min(main_camera->get_current_y() + static_cast<float>((speed * 2) * delta_time), static_cast<float>((int)map->get_map_height() - (int)world->get_max_vertical_tiles() - 1)));
        //main_camera->set_current_y(main_camera->get_current_y() + ((speed * 2) * delta_time));
    }
}

void camera_module::on_late_update(double delta_time)
{

}

void camera_module::on_fixed_update(double fixed_delta_time)
{

}