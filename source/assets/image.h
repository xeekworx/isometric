#pragma once
#include <memory>
#include <string>
#include "asset.h"

namespace isometric::assets {

    class image : public asset
    {
    protected:
        SDL_Surface* surface = nullptr;
        SDL_Texture* texture = nullptr;

        image(const std::string& name, const std::string& path);

    public:
        static std::unique_ptr<image> load(const std::string& name, const std::string& path);

        virtual SDL_Texture* get_texture() const;
        virtual SDL_Surface* get_surface() const;

        unsigned get_width() const;
        unsigned get_height() const;

        SDL_Rect get_rect() const;
        SDL_FRect get_frect() const;

        virtual void clear() override;
        virtual ~image();
    };

}

