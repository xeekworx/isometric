#include "player_module.h"
#include <algorithm>

using namespace isometric;
using namespace isometric::game;

void player_module::on_registered()
{

}

void player_module::on_unregister()
{

}

void player_module::setup(std::shared_ptr<tile_map> map, std::shared_ptr<isometric::world> world)
{
    this->map = map;
    this->world = world;
}

void player_module::set_player_location(const SDL_FPoint& point)
{
    location = point;
}

const SDL_FPoint& player_module::get_player_location() const
{
    return location;
}

void player_module::on_update(double delta_time)
{
    constexpr double speed = 50.0; // Pixels per second of movement

    if (input::keycode_down(SDLK_a))
    {
        location.x -= static_cast<float>(speed * delta_time);
    }
    if (input::keycode_down(SDLK_d))
    {
        location.x += static_cast<float>(speed * delta_time);
    }
    if (input::keycode_down(SDLK_w))
    {
        location.y -= static_cast<float>(speed * delta_time);
    }
    if (input::keycode_down(SDLK_s))
    {
        location.y += static_cast<float>(speed * delta_time);
    }
}

void player_module::on_late_update(double delta_time)
{
    auto graphics = application::get_app()->get_graphics();
    auto renderer = graphics->get_renderer();
    auto main_camera = world->get_main_camera();
    const auto& transform = world->get_transform();

    if (!graphics || !renderer || !main_camera) return;

    constexpr float player_size = 16;

    SDL_FPoint player_in_world{
        location.x,
        location.y
    };

    auto player_in_viewport = transform.world_pixels_to_viewport_pixels(player_in_world);

    SDL_FRect player_rect{
        player_in_viewport.x,
        player_in_viewport.y,
        player_size, player_size
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRectF(renderer, &player_rect);
}

void player_module::on_fixed_update(double fixed_delta_time)
{

}