#include "graphics.h"
#include "../application/application.h"
#include <algorithm>

using namespace isometric;

graphics::graphics(SDL_Renderer* renderer) : renderer(renderer)
{
    pixel_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    asset_manager = application::get_app()->get_asset_manager();
}

graphics::~graphics()
{
    if (pixel_format) SDL_FreeFormat(pixel_format);
}

bool graphics::has_sanity() const
{
    bool sanity = renderer && pixel_format;

    if (!sanity) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "graphics lacks sanity (missing renderer and/or pixel format)");
    }

    return sanity;
}

void graphics::present() const
{
    if (!has_sanity()) return;

    SDL_RenderPresent(renderer);
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
    if (!has_sanity()) return 0x00000000;

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

void graphics::draw_text(const std::string& font_name, const std::string& text, const SDL_FPoint& point, bool align_right)
{
    auto& asset = (*this->asset_manager)[font_name];
    if (!asset) return;

    auto font = dynamic_cast<const isometric::assets::font*>(asset.get());

    SDL_Texture* texture = nullptr;
    SDL_Surface* font_surface = TTF_RenderText_Blended(font->get_font(), text.c_str(), SDL_Color{ 255, 255, 255, 255 });
    float width = static_cast<float>(font_surface->w);
    float height = static_cast<float>(font_surface->h);
    if (font_surface) {
        texture = SDL_CreateTextureFromSurface(renderer, font_surface);
        SDL_FreeSurface(font_surface);
    }

    if (texture) {
        SDL_FRect dest = SDL_FRect{
            !align_right ? point.x : point.x - width,
            point.y,
            width,
            height
        };

        SDL_RenderCopyF(renderer, texture, NULL, &dest);
        SDL_DestroyTexture(texture);
    }
}