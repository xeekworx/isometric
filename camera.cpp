#include "camera.h"
#include <algorithm>

using namespace isometric;

std::shared_ptr<camera> camera::create(unsigned viewport_x, unsigned viewport_y, unsigned width, unsigned height, float start_x, float start_y)
{
    std::shared_ptr<camera> new_camera = std::shared_ptr<camera>(new camera); // std::make_shared<camera>();

    new_camera->set_viewport(viewport_x, viewport_y, width, height);
    new_camera->set_current_pos(start_x, start_y);

    return new_camera;
}

void camera::enable(bool enable)
{
    enabled = enable;
}

void camera::disable()
{
    enable(false);
}

bool camera::is_enabled() const
{
    return enabled;
}

unsigned camera::get_viewport_x() const
{
    return viewport_x;
}

unsigned camera::get_viewport_y() const
{
    return viewport_y;
}

unsigned camera::get_width() const
{
    return viewport_width;
}

unsigned camera::get_height() const
{
    return viewport_height;
}

float camera::get_current_x() const
{
    return current_tile_x;
}

float camera::get_current_y() const
{
    return current_tile_y;
}

void camera::set_viewport(unsigned x, unsigned y, unsigned width, unsigned height)
{
    viewport_x = x;
    viewport_y = y;

    if (width > 0) viewport_width = width;
    if (height > 0) viewport_height = height;
}

void camera::set_current_pos(float tile_x, float tile_y)
{
    set_current_x(tile_x);
    set_current_y(tile_y);
}

void camera::set_current_pos(const SDL_FPoint& point)
{
    set_current_x(point.x);
    set_current_y(point.y);
}

void camera::set_current_x(float tile_x)
{
    current_tile_x = std::max(tile_x, 0.0f);
}

void camera::set_current_y(float tile_y)
{
    current_tile_y = std::max(tile_y, 0.0f);
}