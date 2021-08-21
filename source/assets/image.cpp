#include <SDL.h>
#include <SDL_image.h>
#include "image.h"
#include "../source/application/application.h"

using namespace isometric::assets;

image::image(const std::string& name, SDL_Texture* sdl_texture, SDL_Surface* sdl_surface)
    : asset(name), texture(sdl_texture), surface(sdl_surface)
{

}

image::~image()
{
    clear();
}

void isometric::assets::image::clear()
{
    if (surface) {
        SDL_FreeSurface(surface);
        surface = nullptr;
    }

    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

std::unique_ptr<image> image::load(const std::string& name, const std::string& path)
{
    if (!application::get_app() || !application::get_app()->is_initialized()) {
        auto error_msg = std::string("Attempted to load image before an application object has been created and initialized");
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, error_msg.c_str());
        throw std::exception(error_msg.c_str());
    }

    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load surface for image [%s] from '%s'", name.c_str(), path.c_str());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
        application::get_app()->get_graphics()->get_renderer(),
        surface
    );
    if (texture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create texture for image [%s] from '%s'", name.c_str(), path.c_str());
        SDL_FreeSurface(surface);
        surface = nullptr;
        return nullptr;
    }

    return std::move(std::unique_ptr<image>(new image(name, texture, surface)));
}

SDL_Texture* image::get_texture() const
{
    return texture;
}

SDL_Surface* image::get_surface() const
{
    return surface;
}

unsigned image::get_width() const
{
    SDL_Rect rect = get_rect();
    return rect.w;
}

unsigned image::get_height() const
{
    SDL_Rect rect = get_rect();
    return rect.h;
}

SDL_Rect image::get_rect() const
{
    if (texture) {
        int width = 0, height = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        return SDL_Rect(0, 0, width, height);
    }
    else if (surface) {
        return SDL_Rect(0, 0, surface->w, surface->h);
    }

    return SDL_Rect();
}

SDL_FRect image::get_frect() const
{
    SDL_Rect rect = get_rect();
    return SDL_FRect(
        static_cast<float>(rect.x),
        static_cast<float>(rect.y),
        static_cast<float>(rect.w),
        static_cast<float>(rect.h)
    );
}
