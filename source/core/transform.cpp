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

bool transform::has_sanity() const
{
    bool sanity = main_camera && map;

    if (!sanity)
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "transform lacks sanity (missing camera and/or map)");
    }

    return sanity;
}

SDL_FPoint transform::world_tile_to_world_pixels(const SDL_Point& tile_point) const
{
    if (!has_sanity()) return SDL_FPoint();

    float world_x =
        // The x coordinate is the tile column position multiplied by the pixel width of a tile:
        tile_point.x * map->get_tile_width() +
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

SDL_Point transform::world_pixels_to_world_tile(const SDL_FPoint& point) const
{
    if (!has_sanity()) return SDL_Point();

    int y = static_cast<int>(point.y / (map->get_tile_height() / 2.0f));
    int x = static_cast<int>(point.x / map->get_tile_width() +
        y % 2 == 0
        ? -static_cast<int>(map->get_tile_width()) / 2.0f
        : 0.0f
        );

    // Increase precision of the tile point by doing a hit test of tiles in the general area:
    SDL_Point tile_points[] = {
        { x - 1, y - 1 },   // Top Left
        { x + 1, y - 1 },   // Top Right
        { x - 1, y + 1 },   // Bottom Left
        { x + 1, y + 1 }    // Bottom Right
    };

    for (const auto& tile_point : tile_points)
    {
        if (tile_hittest(tile_point, point))
        {
            return tile_point;
        }
    }

    return SDL_Point{ x, y };
}

SDL_FPoint transform::world_tile_to_viewport_pixels(const SDL_Point& tile_point) const
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

SDL_FPoint transform::world_pixels_to_viewport_pixels(const SDL_FPoint& point) const
{
    if (!has_sanity()) return SDL_FPoint();

    SDL_Point current_tile_pos{
        static_cast<int>(main_camera->get_current_x()),
        static_cast<int>(main_camera->get_current_y())
    };

    SDL_FPoint current_pixel_pos = world_tile_to_world_pixels(current_tile_pos);

    current_pixel_pos.x +=
        current_tile_pos.y % 2 == 0
        ? map->get_tile_width() / 2.0f
        : 0.0f;

    return SDL_FPoint{
        point.x - current_pixel_pos.x,
        point.y - current_pixel_pos.y
    };
}

bool transform::tile_hittest(const SDL_Point& tile_point, const SDL_FPoint& point) const
{
    const SDL_FPoint tile_viewport_point = world_tile_to_viewport_pixels(tile_point);
    return tile_hittest_by_viewport(tile_viewport_point, point);
}

bool transform::tile_hittest_by_viewport(const SDL_FPoint& tile_viewport_point, const SDL_FPoint& point) const
{
    if (!has_sanity()) return false;

    SDL_FPoint translated_point = SDL_FPoint{
        point.x - tile_viewport_point.x,
        point.y - tile_viewport_point.y
    };

    float row_width = 2.0f + translated_point.y * 4.0f;

    if (translated_point.y < map->get_tile_height() / 2.0f)
    { // Top portion of diamond
// The width of the diamond pixels for each row +4 more than the previous one starting at 2
        row_width = 2.0f + translated_point.y * 4.0f;
    }
    else
    { // Lower portion of diamond
        row_width = 2.0f + ((map->get_tile_height() - translated_point.y) - 1) * 4.0f;
    }

    float row_start = map->get_tile_width() / 2.0f - row_width / 2.0f;
    float row_end = row_start + row_width;

    if (translated_point.x >= row_start - 1 && translated_point.x < row_end - 1)
    {
        return true;
    }

    return false;
}