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
    new_tile_map->tiles.resize(static_cast<size_t>(map_width * map_height));

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

unsigned tile_map::add_layer(const std::string& layer_name)
{
    if (std::find(layers.begin(), layers.end(), layer_name) == layers.end() /* Not found */)
    {
        layers.push_back(layer_name);
        return static_cast<unsigned>(layers.size() - 1);
    }
    else
    {
        return get_layer_id(layer_name);
    }
}

const std::vector<std::string>& tile_map::get_layers() const
{
    return layers;
}

unsigned tile_map::get_layer_id(const std::string& layer_name) const
{
    auto iter = std::find(layers.begin(), layers.end(), layer_name);
    if (iter != layers.end())
    {
        return static_cast<unsigned>(std::distance(layers.begin(), iter));
    }
    else 
    {
        return std::numeric_limits<unsigned>::max();
    }
}

const std::string& tile_map::get_layer_name(unsigned layer_id) const
{
    static std::string empty_string = std::string();

    if (layer_id >= layers.size()) return empty_string;
    else
    {
        return layers[layer_id];
    }
}

const std::vector<tile>& tile_map::get_tiles() const
{
    return tiles;
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

tile* tile_map::get_tile(unsigned x, unsigned y)
{
    unsigned tile_index = x + y * get_map_width();

    if (tile_index >= 0 && tile_index < tiles.size()) 
    {
        return &tiles[tile_index];
    }

    return nullptr;
}

tile* tile_map::set_tile(unsigned x, unsigned y, const tile& tile_source)
{
    unsigned tile_index = x + y * get_map_width();

    if (tile_index >= 0 && tile_index < tiles.size())
    {
        tiles[tile_index] = tile_source;
        return &tiles[tile_index];
    }
    else
    {
        return nullptr;
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
        unsigned random_index = random::range(0U, (unsigned)layer_default_images.at(layer_name).size() - 1U);
        return layer_default_images.at(layer_name)[random_index];
    }
    else
    {
        return 0;
    }
}

unsigned tile_map::get_random_layer_default_image(unsigned layer_id) const
{
    return get_random_layer_default_image(get_layer_name(layer_id));
}

bool tile_map::layer_has_default_images(const std::string& layer_name) const
{
    return 
        layer_default_images.contains(layer_name) && 
        layer_default_images.at(layer_name).size() > 0;
}

bool tile_map::layer_has_default_images(unsigned layer_id) const
{
    if (layer_id >= layers.size()) return false;
    else {
        auto &layer_name = layers[layer_id];
        return layer_has_default_images(layer_name);
    }
}