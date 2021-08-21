#include <SDL.h>
#include <SDL_ttf.h>
#include "font.h"
#include "../source/application/application.h"

using namespace isometric::assets;

font::font(const std::string& name)
    : asset(name)
{

}

font::~font()
{
    clear();
}

std::unique_ptr<font> font::load(const std::string& name, const std::string& path, const std::vector<int>& point_sizes)
{
    if (!application::get_app() || !application::get_app()->is_initialized()) {
        auto error_msg = std::string("Attempted to load image before an application object has been created and initialized");
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, error_msg.c_str());
        throw std::exception(error_msg.c_str());
    }

    auto new_font = std::unique_ptr<font>(new font(name));

    for (int point_size : point_sizes) {
        TTF_Font* sdl_font = TTF_OpenFont(path.c_str(), point_size);
        if (sdl_font == NULL) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load font named [%s] with point size [%d] from '%s'", name.c_str(), point_size, path.c_str());
            new_font.reset();
            return nullptr;
        }

        new_font->fonts[point_size] = sdl_font;
    }

    return std::move(new_font);
}

std::unique_ptr<font> font::load(const std::string& name, const std::string& path, int point_size)
{
    auto sizes = std::vector<int>{ point_size };
    return std::move(load(name, path, sizes));
}

TTF_Font* font::get_font(int point_size) const
{
    if (fonts.empty()) {
        return nullptr;
    }
    else if (fonts.contains(point_size)) {
        return fonts.at(point_size);
    }
    else {
        return (*fonts.begin()).second;
    }
}

void font::clear()
{
    for (auto& pair : fonts) {
        int point_size = pair.first;
        TTF_Font* sdl_font = pair.second;

        if (sdl_font) {
            TTF_CloseFont(sdl_font);
            sdl_font = nullptr;
        }
    }

    fonts.clear();
}
