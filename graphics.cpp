#include "graphics.h"

using namespace isometric;

graphics::graphics(SDL_Renderer* renderer) : renderer(renderer)
{

}

void graphics::clear(uint32_t color) const
{
    if (!renderer) return;

    SDL_Color draw_color{};
    auto pixel_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    SDL_GetRGBA(
        color, pixel_format,
        &draw_color.r,
        &draw_color.g,
        &draw_color.b,
        &draw_color.a
    );

    SDL_SetRenderDrawColor(
        renderer, 
        draw_color.r,
        draw_color.g, 
        draw_color.b, 
        draw_color.a
    );

    SDL_RenderClear(renderer);
}
