#pragma once
#include <string>
#include <memory>
#include <SDL.h>

namespace isometric {

    class tile_image
    {
    private:
        unsigned image_id = 0;
        std::string name;

        SDL_Texture * texture = NULL;

        unsigned source_x = 0;
        unsigned source_y = 0;
        unsigned source_w = 0;
        unsigned source_h = 0;

        tile_image() { }

    public:

        static std::shared_ptr<tile_image> create(unsigned image_id, SDL_Texture* texture, unsigned source_x, unsigned source_y, unsigned source_w, unsigned source_h)
        {
            return create(std::string(), image_id, texture, source_x, source_y, source_w, source_h);
        }

        static std::shared_ptr<tile_image> create(std::string name, unsigned image_id, SDL_Texture* texture, unsigned source_x, unsigned source_y, unsigned source_w, unsigned source_h)
        {
            auto image = std::shared_ptr<tile_image>(new tile_image);

            image->name = name;
            image->image_id = image_id;

            image->texture = texture;

            image->source_x = source_x;
            image->source_y = source_y;

            image->source_w = source_w;
            image->source_h = source_h;

            return image;
        }

        unsigned get_image_id() const
        {
            return image_id;
        }

        SDL_Texture * get_texture() const
        {
            return texture;
        }

        const bool has_texture() const
        {
            return texture != nullptr;
        }

        unsigned get_source_x() const
        {
            return source_x;
        }

        unsigned get_source_y() const
        {
            return source_y;
        }

        unsigned get_source_w() const
        {
            return source_w;
        }

        unsigned get_source_h() const
        {
            return source_h;
        }

        const SDL_Rect* get_source_rect() const;

        const SDL_FRect* get_dest_rect(float x, float y, unsigned tile_height = 0) const;

        bool is_empty() const
        {
            return texture == NULL;
        }
    };
}
