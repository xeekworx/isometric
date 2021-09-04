#include <SDL.h>
#include <SDL_image.h>
#include "image_atlas.h"
#include "../source/application/application.h"

using namespace isometric::assets;

image_atlas::image_atlas(const std::string& name, const std::string& path)
    : image(name, path)
{

}

image_atlas::~image_atlas()
{
    clear();
}

void image_atlas::clear()
{
    subimage_names.clear();
    subimages.clear();
    image::clear();
}

std::unique_ptr<image_atlas> image_atlas::load(const std::string& name, const std::string& path)
{
    try
    {
        return std::move(std::unique_ptr<image_atlas>(new image_atlas(name, path)));
    }
    catch (std::exception ex)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, ex.what());
        return nullptr;
    }
}

void image_atlas::generate_subimages(unsigned width, unsigned height)
{
    unsigned atlas_width = image::get_width();
    unsigned atlas_height = image::get_width();

    for (unsigned y = 0; y < atlas_height; y += height)
    {
        for (unsigned x = 0; x < atlas_width; x += width)
        {
            SDL_Rect srcrect{
                static_cast<int>(x),
                static_cast<int>(y),
                static_cast<int>(width),
                static_cast<int>(height)
            };

            subimages.push_back(srcrect);
        }
    }
}

size_t image_atlas::set_subimage(const SDL_Rect & srcrect, const std::string & name)
{
    size_t index = subimages.size();
    subimages.push_back(srcrect);
    set_subimage_name(index, name);

    return index;
}

void image_atlas::set_subimage_name(size_t index, const std::string& name)
{
    if (index >= subimages.size() || name.empty())
    {
        return;
    }
    else
    {
        subimage_names[name] = index;
    }
}

size_t image_atlas::get_subimage_index(const std::string & name, size_t default_index) const
{
    if (subimage_names.contains(name))
    {
        return subimage_names.at(name);
    }
    else
    {
        return default_index;
    }
}

const SDL_Rect& image_atlas::get_subimage(const std::string& name) const
{
    if (subimage_names.contains(name))
    {
        return get_subimage(subimage_names.at(name));
    }
    else
    {
        return empty_rect;
    }
}

const SDL_Rect& image_atlas::get_subimage(size_t index) const
{
    if (index < subimages.size())
    {
        return subimages[index];
    }
    else
    {
        return empty_rect;
    }
}

size_t image_atlas::subimage_count() const
{
    return subimages.size();
}
