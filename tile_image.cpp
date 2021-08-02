#include "tile_image.h"

using namespace isometric;

const SDL_Rect* tile_image::get_source_rect() const
{
    static SDL_Rect tmp_rect = { 0 };

    tmp_rect = {
        static_cast<int>(source_x),
        static_cast<int>(source_y),
        static_cast<int>(source_w),
        static_cast<int>(source_h)
    };

    return &tmp_rect;
}

const SDL_FRect* tile_image::get_dest_rect(float x, float y, unsigned tile_height) const
{
    static SDL_FRect tmp_rect = { 0 };

    unsigned y_negative_offset = tile_height > 0 ? source_h - tile_height : 0;

    tmp_rect = {
        x,
        y - y_negative_offset,
        static_cast<float>(source_w),
        static_cast<float>(source_h)
    };

    return &tmp_rect;
}