#include "tile_map.h"
#include "random.h"
#include <limits>

using namespace isometric;

std::shared_ptr<tile_map> tile_map::create(unsigned map_width, unsigned map_height, unsigned tile_width, unsigned tile_height)
{
    std::shared_ptr<tile_map> new_tile_map = std::shared_ptr<tile_map>(new tile_map);

    new_tile_map->map_width = map_width;
    new_tile_map->map_height = map_height;
    new_tile_map->tile_width = tile_width;
    new_tile_map->tile_height = tile_height;

    return new_tile_map;
}

unsigned tile_map::add_image(std::shared_ptr<tile_image> image)
{
    tile_images[image->get_image_id()] = image;
    return image->get_image_id();
}

std::shared_ptr<tile_image> tile_map::get_image(unsigned id) const
{
    if (tile_images.contains(id)) 
    {
        return tile_images.at(id);
    }
    else 
    {
        return nullptr;
    }
}

void tile_map::set_selection_image(unsigned id)
{
    selection_tile_image = id;
}

bool tile_map::has_selection_image() const
{
    return tile_images.contains(selection_tile_image);
}

std::shared_ptr<tile_image> tile_map::get_selection_image() const
{
    if (has_selection_image()) 
    {
        return tile_images.at(selection_tile_image);
    }
    else
    {
        return nullptr;
    }
}

void tile_map::add_layer(const std::string& name)
{
    if (!layer_tiles.contains(name)) {
        layer_tiles[name] = std::vector<tile>(
            map_width * map_height,
            tile()
        );

        layers.push_back(name);
    }
}

const std::vector<std::string>& tile_map::get_layers() const
{
    return layers;
}

unsigned tile_map::get_layer_zindex(const std::string& layer_name) const
{
    for (unsigned zindex = 0; zindex < layers.size(); zindex++)
    {
        if (layers[zindex] == layer_name) return zindex;
    }

    return std::numeric_limits<unsigned>::max();
}

bool tile_map::add_tile(const std::string& layer_name, unsigned map_x, unsigned map_y, const tile& tile)
{
    if (map_x >= map_width || map_y >= map_height) return false;

    if (!layer_tiles.contains(layer_name))
    {
        add_layer(layer_name);
    }

    layer_tiles[layer_name][map_x + map_width * static_cast<size_t>(map_y)] = tile;

    return true;
}

const tile& tile_map::get_tile(const std::string& layer_name, unsigned map_x, unsigned map_y)
{
    if (map_x >= map_width || map_y >= map_height || !layer_tiles.contains(layer_name)) {
        return tile();
    }

    return layer_tiles.at(layer_name)[map_x + map_width * static_cast<size_t>(map_y)];
}

const std::vector<tile>& tile_map::get_tiles(const std::string& layer_name) const
{
    return layer_tiles.at(layer_name);
}

unsigned tile_map::get_tile_width() const
{
    return tile_width;
}

unsigned tile_map::get_tile_height() const
{
    return tile_height;
}

unsigned tile_map::get_map_width() const
{
    return map_width;
}

unsigned tile_map::get_map_height() const
{
    return map_height;
}

const tile& tile_map::get_tile(const std::string& layer, unsigned x, unsigned y) const
{
    unsigned tile_index = x + y * get_map_width();

    if (layer_tiles.contains(layer) && tile_index >= 0 && tile_index < layer_tiles.size()) {
        return layer_tiles.at(layer)[tile_index];
    }

    return tile();
}

void tile_map::set_tile(const std::string& layer, unsigned x, unsigned y, const tile& tile_source)
{
    unsigned tile_index = x + y * get_map_width();

    if (layer_tiles.contains(layer) && tile_index >= 0 && tile_index < layer_tiles.at(layer).size()) {
        layer_tiles.at(layer)[tile_index] = tile_source;
    }
}

void tile_map::add_layer_default_image(const std::string& layer_name, unsigned image_id)
{
    layer_default_images[layer_name].push_back(image_id);
}

const std::vector<unsigned>& tile_map::get_layer_default_images(const std::string& layer_name) const
{
    return layer_default_images.at(layer_name);
}

unsigned tile_map::get_random_layer_default_image(const std::string& layer_name) const
{
    if (layer_has_default_images(layer_name)) 
    {
        unsigned random_index = random::range(0, layer_default_images.at(layer_name).size() - 1);
        return layer_default_images.at(layer_name)[random_index];
    }
    else
    {
        return 0;
    }
}

bool tile_map::layer_has_default_images(const std::string& layer_name) const
{
    return 
        layer_default_images.contains(layer_name) && 
        layer_default_images.at(layer_name).size() > 0;
}