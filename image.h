#pragma once
#include <memory>
#include <string>

namespace isometric {

    class image
    {
    private:
        std::string name;
        SDL_Surface* surface = nullptr;
        SDL_Texture* texture = nullptr;

        image(const std::string& name, SDL_Texture* sdl_texture, SDL_Surface* sdl_surface);
        

    public:
        static std::shared_ptr<image> load(const std::string& name, const std::string& path);

        SDL_Texture* get_texture();
        SDL_Surface* get_surface();

        unsigned get_width() const;
        unsigned get_height() const;

        SDL_Rect get_rect() const;
        SDL_FRect get_frect() const;

        ~image();
    };

}

