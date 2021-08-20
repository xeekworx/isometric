#pragma once
#include "camera.h"
#include "tile_map.h"

namespace isometric {

    class transform
    {
    private:
        std::shared_ptr<camera> main_camera;
        std::shared_ptr<tile_map> map;

        float world_x = 0;
        float world_y = 0;

    public:
        transform(std::shared_ptr<camera> camera, std::shared_ptr<tile_map> map);

        std::shared_ptr<camera> get_camera() const;
        std::shared_ptr<camera> set_camera(std::shared_ptr<camera> camera);

        std::shared_ptr<tile_map> get_map() const;
        std::shared_ptr<tile_map> set_map(std::shared_ptr<tile_map> map);

        bool has_sanity() const;

        /// <summary>
        /// Converts a world tile position (in tile coordinates) to a world based pixel position. The pixel based 
        /// coordinates starts at 0, 0 relative to the top left of the whole map.
        /// </summary>
        /// <param name="tile_point">The position in tile coordinates, not pixel coordinates</param>
        /// <returns>The pixel position of a tile starting at 0, 0</returns>
        SDL_FPoint world_tile_to_world_pixels(const SDL_Point& tile_point) const;

        /// <summary>
        /// Converts a world based pixel position to a world tile position (in tile coordinates). The pixel based 
        /// coordinates starts at 0, 0 relative to the top left of the whole map.
        /// </summary>
        /// <param name="point">The position in world pixel coordinates, not tile coordinates</param>
        /// <returns>The tile position of a tile starting at 0, 0 relative to the entire world</returns>
        SDL_Point world_pixels_to_world_tile(const SDL_FPoint& point) const;

        /// <summary>
        /// Converts a world tile position (in tile coordinates) to a viewport pixel position. This is where the tile
        /// should be rendered to the viewport based on the top left of what is visible on the screen.
        /// </summary>
        /// <param name="tile_point">The world position in tile coordinates, not pixel coordinates</param>
        /// <returns>The pixel position of a tile starting at top left of the camera viewport</returns>
        SDL_FPoint world_tile_to_viewport_pixels(const SDL_Point& tile_point) const;

        /// <summary>
        /// Converts a world (pixels) point into a viewport (pixels) point.
        /// </summary>
        /// <param name="point">The world position in pixel coordinates, not tile coordinates</param>
        /// <returns>The pixel position starting at top left of the camera viewport</returns>
        SDL_FPoint world_pixels_to_viewport_pixels(const SDL_FPoint& point) const;

        /// <summary>
        /// Is an viewport x, y point inside a tile
        /// </summary>
        /// <param name="tile_point">The position in tile coordinates, not pixel coordinates</param>
        /// <param name="point">The pixel coordinate to test</param>
        /// <returns>True if point is inside the tile</returns>
        bool tile_hittest(const SDL_Point& tile_point, const SDL_FPoint& point) const;

        /// <summary>
        /// Is an viewport point inside a tile from a viewport point
        /// </summary>
        /// <param name="tile_viewport_point">
        /// The tile position in screen (pixel) coordinates, not tile coordinates
        /// </param>
        /// <param name="point">The pixel coordinate to test</param>
        /// <returns>True if point is inside the tile</returns>
        bool tile_hittest_by_viewport(const SDL_FPoint& tile_viewport_point, const SDL_FPoint& point) const;
    };

}

