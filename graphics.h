#pragma once
#include <SDL.h>

namespace isometric {

    class application;

    class graphics
    {
        friend class application;
    private:
        SDL_Renderer * renderer = nullptr;

        graphics(SDL_Renderer* renderer);
        void present() const;

    public:

        void clear(uint32_t color) const;
        SDL_Renderer* get_renderer() { return renderer; }

    };

}