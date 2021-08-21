#pragma once
#include <memory>
#include <string>
#include "asset.h"

namespace isometric::assets {

    class image : public asset
    {
    private:
        SDL_Surface* surface = nullptr;
        SDL_Texture* texture = nullptr;

        image(const std::string& name, SDL_Texture* sdl_texture, SDL_Surface* sdl_surface);

    public:
        static std::unique_ptr<image> load(const std::string& name, const std::string& path);

        SDL_Texture* get_texture() const;
        SDL_Surface* get_surface() const;

        unsigned get_width() const;
        unsigned get_height() const;

        SDL_Rect get_rect() const;
        SDL_FRect get_frect() const;

        void clear() override;
        virtual ~image();
    };

}

