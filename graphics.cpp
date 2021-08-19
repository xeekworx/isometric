#include "graphics.h"
#include <algorithm>

using namespace isometric;

graphics::graphics(SDL_Renderer* renderer) : renderer(renderer)
{
    pixel_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
}

graphics::~graphics()
{
    if (pixel_format) SDL_FreeFormat(pixel_format);
}

bool graphics::has_sanity() const
{
    bool sanity = renderer && pixel_format;

    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "graphics lacks sanity (missing renderer and/or pixel format)");

    return sanity;
}

void graphics::set_color(uint32_t color)
{
    if (!has_sanity()) return;

    SDL_Color draw_color{};
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
}

uint32_t isometric::graphics::get_color()
{
    if (!has_sanity()) return;

    SDL_Color draw_color{};
    SDL_GetRenderDrawColor(renderer,
        &draw_color.r,
        &draw_color.g,
        &draw_color.b,
        &draw_color.a
    );

    return
        draw_color.a +
        (draw_color.b << 8) +
        (draw_color.g << 16) +
        (draw_color.r << 24);
}

void graphics::clear()
{
    if (!has_sanity()) return;

    SDL_RenderClear(renderer);
}

void graphics::clear(uint32_t color)
{
    if (!has_sanity()) return;

    set_color(color);

    SDL_RenderClear(renderer);
}

void graphics::present() const
{
    if (!has_sanity()) return;

    SDL_RenderPresent(renderer);
}