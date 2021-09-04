#include "graphics.h"
#include "../application/application.h"
#include <algorithm>

using namespace isometric::rendering;

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

uint32_t graphics::get_color() const
{
    SDL_Color draw_color = get_sdl_color();

    return
        draw_color.a +
        (draw_color.b << 8) +
        (draw_color.g << 16) +
        (draw_color.r << 24);
}

SDL_Color graphics::get_sdl_color() const
{
    if (!has_sanity()) return { 0 };

    SDL_Color draw_color{};
    SDL_GetRenderDrawColor(renderer,
        &draw_color.r,
        &draw_color.g,
        &draw_color.b,
        &draw_color.a
    );

    return draw_color;
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

SDL_FRect graphics::size_text(
    const std::string& font_name, int point_size,
    const std::string& text,
    const SDL_FPoint& point
)
{
    auto& asset = (*this->asset_manager)[font_name];
    if (!asset) return { 0 };

    auto font = dynamic_cast<const isometric::assets::font*>(asset.get());

    int width = 0, height = 0;
    TTF_SizeUTF8(font->get_font(), text.c_str(), &width, &height);

    return SDL_FRect{ point.x, point.y, static_cast<float>(width), static_cast<float>(height) };
}

void graphics::draw_text(
    const std::string& font_name, int point_size,
    const std::string& text,
    const SDL_Point& point,
    content_align align
)
{
    auto& asset = (*this->asset_manager)[font_name];
    if (!asset) return;

    auto font = dynamic_cast<const isometric::assets::font*>(asset.get());

    SDL_Texture* texture = nullptr;
    SDL_Surface* font_surface = nullptr;

    font_surface = TTF_RenderUTF8_Blended(font->get_font(point_size), text.c_str(), get_sdl_color());

    int text_width = font_surface->w;
    int text_height = font_surface->h;

    if (font_surface) {
        texture = SDL_CreateTextureFromSurface(renderer, font_surface);
        SDL_FreeSurface(font_surface);
    }

    if (!texture) return;

    SDL_Rect dest = SDL_Rect{ point.x, point.y, text_width, text_height };

    // Horizontal Alignment:
    switch (align) {
    case content_align::top_center:
    case content_align::middle_center:
    case content_align::bottom_center:
        dest.x += static_cast<int>(std::round(text_width / 2.0f));
        break;
    case content_align::top_right:
    case content_align::middle_right:
    case content_align::bottom_right:
        dest.x += text_width;
        break;
    }
    
    // Vertical Alignment:
    switch (align) {
    case content_align::middle_left:
    case content_align::middle_center:
    case content_align::middle_right:
        dest.y += static_cast<int>(std::round(text_height / 2.0f));
        break;
    case content_align::bottom_left:
    case content_align::bottom_center:
    case content_align::bottom_right:
        dest.y += text_height;
        break;
    }

    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
}

void graphics::draw_text(
    const std::string& font_name, int point_size,
    const std::string& text,
    const SDL_Rect& destination,
    content_align align,
    bool wrap
)
{
    auto& asset = (*this->asset_manager)[font_name];
    if (!asset) return;

    auto font = dynamic_cast<const isometric::assets::font*>(asset.get());

    SDL_Texture* texture = nullptr;
    SDL_Surface* font_surface = nullptr;

    if (wrap) {
        font_surface = TTF_RenderUTF8_Blended_Wrapped(font->get_font(point_size), text.c_str(), SDL_Color{ 255, 255, 255, 255 }, destination.w);
    }
    else {
        font_surface = TTF_RenderUTF8_Blended(font->get_font(point_size), text.c_str(), SDL_Color{ 255, 255, 255, 255 });
    }

    int text_width = font_surface->w;
    int text_height = font_surface->h;

    if (font_surface) {
        texture = SDL_CreateTextureFromSurface(renderer, font_surface);
        SDL_FreeSurface(font_surface);
    }

    if (!texture) return;

    SDL_Rect real_dest = SDL_Rect{ destination.x, destination.y, text_width, text_height };

    // Horizontal Alignment:
    switch (align) {
    case content_align::top_center:
    case content_align::middle_center:
    case content_align::bottom_center:
        real_dest.x += static_cast<int>(std::round(destination.w / 2.0 - text_width / 2.0));
        break;
    case content_align::top_right:
    case content_align::middle_right:
    case content_align::bottom_right:
        real_dest.x += destination.w - text_width;
        break;
    }

    // Vertical Alignment:
    switch (align) {
    case content_align::middle_left:
    case content_align::middle_center:
    case content_align::middle_right:
        real_dest.y += static_cast<int>(std::round(destination.h / 2.0 - text_height / 2.0));
        break;
    case content_align::bottom_left:
    case content_align::bottom_center:
    case content_align::bottom_right:
        real_dest.y += destination.h - text_height;
        break;
    }

    SDL_Color sdl_color = get_sdl_color();
    SDL_SetTextureColorMod(texture, sdl_color.r, sdl_color.g, sdl_color.b);
    SDL_SetTextureAlphaMod(texture, sdl_color.a);

    SDL_RenderCopy(renderer, texture, NULL, &real_dest);
    SDL_DestroyTexture(texture);
}