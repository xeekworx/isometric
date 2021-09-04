#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "../assets/asset_management.h"
#include "../enumerations/content_align.h"

namespace isometric::rendering {

    class isometric::application;

    class graphics
    {
        friend class application;
    private:
        SDL_Renderer* renderer = nullptr;
        SDL_PixelFormat* pixel_format = nullptr;
        std::shared_ptr<isometric::assets::asset_management> asset_manager = nullptr;

        graphics(SDL_Renderer* renderer);
        void present() const;
        bool has_sanity() const;

    public:
        virtual ~graphics();
        SDL_Renderer* get_renderer() { return renderer; }

        void set_color(uint32_t color);
        uint32_t get_color() const;
        SDL_Color get_sdl_color() const;

        void clear();
        void clear(uint32_t color);

        SDL_FRect size_text(
            const std::string& font_name, int point_size,
            const std::string& text,
            const SDL_FPoint& point = SDL_FPoint{ 0,0 }
        );

        void draw_text(
            const std::string& font_name, int point_size,
            const std::string& text,
            const SDL_Point& point,
            content_align align = content_align::top_left
        );

        void draw_text(
            const std::string& font_name, int point_size,
            const std::string& text,
            const SDL_Rect& destination,
            content_align align = content_align::top_left,
            bool wrap = false
        );
    };

}