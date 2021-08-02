#include "transform.h"
#include <cmath>

using namespace isometric;

transform::transform(std::shared_ptr<camera> camera, std::shared_ptr<tile_map> map)
    : main_camera(camera), map(map)
{

}

std::shared_ptr<camera> transform::get_camera() const
{
    return main_camera;
}

std::shared_ptr<camera> transform::set_camera(std::shared_ptr<camera> camera)
{
    std::shared_ptr<isometric::camera> old_camera = main_camera;
    main_camera = camera;
    return old_camera;
}

std::shared_ptr<tile_map> transform::get_map() const
{
    return map;
}

std::shared_ptr<tile_map> transform::set_map(std::shared_ptr<tile_map> map)
{
    std::shared_ptr<tile_map> old_map = this->map;
    this->map = map;
    return old_map;
}

SDL_FPoint transform::get_mouse_point()
{
    int mouse_x = 0, mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    return
    {
        static_cast<float>(mouse_x),
        static_cast<float>(mouse_y),
    };
}

bool transform::has_sanity() const
{
    return main_camera && map;
}

SDL_Point transform::screen_tile_to_world_tile(const SDL_Point& tile_point) const
{
    if (!has_sanity()) return SDL_Point();

    // World tile position in tile coordinates:
    return SDL_Point{
        static_cast<int>(main_camera->get_current_x()) + tile_point.x,
        static_cast<int>(main_camera->get_current_y()) + tile_point.y
    };
}

SDL_Point transform::world_tile_to_screen_tile(const SDL_Point& tile_point) const
{
    if (!has_sanity()) return SDL_Point();

    // Screen tile position in tile coordinates:
    return SDL_Point{
        tile_point.x - static_cast<int>(main_camera->get_current_x()),
        tile_point.y - static_cast<int>(main_camera->get_current_y())
    };
}

SDL_FPoint transform::world_tile_to_world_pixels(const SDL_Point& tile_point) const
{
    if (!has_sanity()) return SDL_FPoint();

    float world_x =
        // The x coordinate is the tile column position multiplied by the pixel width of a tile:
        tile_point.x * map->get_tile_width()
        +
        (
            static_cast<int>(tile_point.y) % 2 == 0             // Is this an even tile?
            ? -static_cast<int>(map->get_tile_width()) / 2.0f   // True:  Offset the tile half a tile to the left
            : 0.0f                                              // False: Do not offset the tile
            );

    // The y coordinates are multiplied by half the tile size because tiles in an isometric map are partially
    // overlapping.
    float world_y = tile_point.y * (map->get_tile_height() / 2.0f);

    // Shift the world up half a tile to cover empty spaces:
    world_y -= map->get_tile_height() / 2.0f;

    return SDL_FPoint{ world_x, world_y };
}

SDL_FPoint transform::world_tile_to_viewport(const SDL_Point& tile_point) const
{
    if (!has_sanity()) return SDL_FPoint();

    // Get this tile's pixel position relative to the entire map:
    SDL_FPoint screen_point = world_tile_to_world_pixels(tile_point);

    // Convert the tile's pixel position to be relative to the top left of the camera's position:
    screen_point.x = screen_point.x - main_camera->get_current_x() * map->get_tile_width();
    screen_point.y = screen_point.y - main_camera->get_current_y() * (map->get_tile_height() / 2.0f);

    // Adjust the x, y based on the current camera viewport and not assume the world is drawn starting at 0, 0
    // pixels all of the time.
    screen_point.x += main_camera->get_viewport_x();
    screen_point.y += main_camera->get_viewport_y();

    return screen_point;
}

bool transform::tile_hittest(const SDL_Point& tile_point, const SDL_FPoint& point) const
{
    const SDL_FPoint tile_viewport_point = world_tile_to_viewport(tile_point);
    return tile_hittest_by_viewport(tile_viewport_point, point);
}

bool transform::tile_hittest_by_viewport(const SDL_FPoint& tile_viewport_point, const SDL_FPoint& point) const
{
    SDL_FPoint translated_point = SDL_FPoint{
        point.x - tile_viewport_point.x,
        point.y - tile_viewport_point.y
    };

    float row_width = 2.0f + translated_point.y * 4.0f;

    if (translated_point.y < map->get_tile_height() / 2.0f) { // Top portion of diamond
        // The width of the diamond pixels for each row +4 more than the previous one starting at 2
        row_width = 2.0f + translated_point.y * 4.0f;
    }
    else { // Lower portion of diamond
        row_width = 2.0f + ((map->get_tile_height() - translated_point.y) - 1) * 4.0f;
    }

    float row_start = map->get_tile_width() / 2.0f - row_width / 2.0f;
    float row_end = row_start + row_width;

    if (translated_point.x >= row_start - 1 && translated_point.x < row_end - 1) {
        return true;
    }

    return false;
}