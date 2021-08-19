#pragma once
#include <SDL.h>

namespace isometric {

    class application;

    class graphics
    {
        friend class application;
    private:
        SDL_Renderer* renderer = nullptr;
        SDL_PixelFormat* pixel_format = nullptr;

        graphics(SDL_Renderer* renderer);
        void present() const;

        bool has_sanity() const;

    public:
        virtual ~graphics();

        void set_color(uint32_t color);
        uint32_t get_color();
        void clear();
        void clear(uint32_t color);
        SDL_Renderer* get_renderer() { return renderer; }

    };

}