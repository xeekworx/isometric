#pragma once
#include <SDL.h>

namespace isometric {

    class application;

    class graphics
    {
        friend class application;
    private:
        SDL_Renderer* renderer = nullptr;

        graphics(SDL_Renderer* renderer);

    public:

        void clear(uint32_t color) const;

    };

}