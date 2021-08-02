#pragma once
#include <map>
#include <string>
#include <vector>
#include <memory>>
#include "tile_image.h"
#include "tile.h"

namespace isometric {

    class tile_map
    {
    private:
        unsigned map_width = 0;     // by tiles
        unsigned map_height = 0;    // by tiles
        unsigned tile_width = 0;    // by pixels
        unsigned tile_height = 0;   // by pixels

        std::map<unsigned, std::shared_ptr<tile_image>> tile_images;
        unsigned selection_tile_image = std::numeric_limits<unsigned>::max();
        std::map<std::string, std::vector<tile>> layer_tiles;
        std::map<std::string, std::vector<unsigned>> layer_default_images;
        std::vector<std::string> layers;

        tile_map() { }

    public:
        static std::shared_ptr<tile_map> create(unsigned map_width, unsigned map_height, unsigned tile_width, unsigned tile_height);

        /// <Returns>
        /// Returns the width of the map (total tiles per row)
        /// </Returns>
        unsigned get_map_width() const;

        /// <returns>
        /// Returns the height of the map (total tiles per column)
        /// </returns>
        unsigned get_map_height() const;

        /// <summary>
        /// Returns the width of a single tile in pixels
        /// </summary>
        unsigned get_tile_width() const;

        /// <summary>
        /// Returns the height of a single tile in pixels
        /// </summary>
        unsigned get_tile_height() const;

        /// <summary>
        /// Add an image that this map can use for tiles
        /// </summary>
        /// <param name="image">A valid tile image within a shared_ptr</param>
        unsigned add_image(std::shared_ptr<tile_image> image);

        /// <summary>
        /// Get an image that this map uses for tiles
        /// </summary>
        /// <param name="id">The id of the image to return</param>
        /// <returns>A valid tile image within a shared_ptr</returns>
        std::shared_ptr<tile_image> get_image(unsigned id) const;

        void set_selection_image(unsigned id);
        bool has_selection_image() const;
        std::shared_ptr<tile_image> get_selection_image() const;

        /// <summary>
        /// Set the image id used as the default for tiles that do not have an image in this layer
        /// </summary>
        /// <param name="layer_name">The layer this default is set in</param>
        /// <param name="image_id">The id of the image to use as the default tile image</param>
        void add_layer_default_image(const std::string& layer_name, unsigned image_id);

        /// <summary>
        /// Gets all of the default tile images used for a layer
        /// </summary>
        /// <param name="layer_name">The layer associated with these default tile images</param>
        /// <returns>A vector of tile image ids</returns>
        const std::vector<unsigned>& get_layer_default_images(const std::string& layer_name) const;

        /// <summary>
        /// Get a random default tile image set for a layer
        /// </summary>
        /// <param name="layer_name">The layer associated with this random default tile image</param>
        /// <remarks>If this layer does not have a default
        /// <returns>The id of the image to use as the default tile image</returns>
        unsigned get_random_layer_default_image(const std::string& layer_name) const;

        /// <summary>
        /// Determine if the layer has any default tile images
        /// </summary>
        /// <param name="layer_name">The layer name</param>
        /// <returns>True if the layer has default tile images</returns>
        bool layer_has_default_images(const std::string& layer_name) const;

        void add_layer(const std::string& name);
        const std::vector<std::string>& get_layers() const;
        unsigned get_layer_zindex(const std::string& layer_name) const;

        bool add_tile(const std::string& layer_name, unsigned map_x, unsigned map_y, const tile& tile);
        const tile& get_tile(const std::string& layer_name, unsigned map_x, unsigned map_y);
        const std::vector<tile>& get_tiles(const std::string& layer_name) const;

        const tile& get_tile(const std::string& layer, unsigned x, unsigned y) const;
        void set_tile(const std::string& layer, unsigned x, unsigned y, const tile& tile_source);
    };

}