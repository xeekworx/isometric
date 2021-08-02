#pragma once
#include "camera.h"
#include "tile_map.h"

namespace isometric {
    class transform
    {
    private:
        std::shared_ptr<camera> main_camera;
        std::shared_ptr<tile_map> map;

    public:
        transform(std::shared_ptr<camera> camera, std::shared_ptr<tile_map> map);

        std::shared_ptr<camera> get_camera() const;
        std::shared_ptr<camera> set_camera(std::shared_ptr<camera> camera);

        std::shared_ptr<tile_map> get_map() const;
        std::shared_ptr<tile_map> set_map(std::shared_ptr<tile_map> map);

        static SDL_FPoint get_mouse_point();

        bool has_sanity() const;

        /// <summary>
        /// Converts a tile position on the screen (starting at 0, 0) to a world position in the map based on the 
        /// current position of the main camera.
        /// </summary>
        /// <param name="tile_point">The position on the screen in tile coordinates, not pixel coordinates</param>
        /// <returns>The position of a tile within the world</returns>
        SDL_Point screen_tile_to_world_tile(const SDL_Point& tile_point) const;

        /// <summary>
        /// Converts a world position (in tile coordinates) to a tile position on the screen based on the current
        /// position of the main camera.
        /// </summary>
        /// <param name="tile_point">The position in the world in tile coordinates, not pixel coordinates</param>
        /// <returns>The position of a tile on the screen</returns>
        SDL_Point world_tile_to_screen_tile(const SDL_Point& tile_point) const;

        /// <summary>
        /// Converts a world tile position (in tile coordinates) to a world based pixel position. The pixel based 
        /// coordinates starts at 0, 0 relative to the top left of the whole map.
        /// </summary>
        /// <param name="tile_point">The position in tile coordinates, not pixel coordinates</param>
        /// <returns>The pixel position of a tile starting at 0, 0</returns>
        SDL_FPoint world_tile_to_world_pixels(const SDL_Point& tile_point) const;

        /// <summary>
        /// Converts a world tile position (in tile coordinates) to a viewport pixel position. This is where the tile
        /// should be rendered to the viewport based on the top left of what is visible on the screen.
        /// </summary>
        /// <param name="tile_point">The world position in tile coordinates, not pixel coordinates</param>
        /// <returns>The pixel position of a tile starting at top left of the camera viewport</returns>
        SDL_FPoint world_tile_to_viewport(const SDL_Point& tile_point) const;

        bool tile_hittest(const SDL_Point& tile_point, const SDL_FPoint& point) const;

        bool tile_hittest_by_viewport(const SDL_FPoint& tile_viewport_point, const SDL_FPoint& point) const;
    };
}

