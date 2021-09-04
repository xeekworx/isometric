#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <unordered_map>
#include <vector>
#include "../enumerations/content_align.h"

namespace isometric::rendering {

    struct glyph_info
    {
        SDL_Surface* surface; // Only used for generation
        SDL_Rect srcrect;
        size_t texture_index;
    };

    struct bitmap_font_info {
        std::vector<std::tuple<SDL_Texture*, SDL_Rect>> textures;
        std::unordered_map<char, glyph_info> glyphs;
    };

    class simple_bitmap_font
    {
    private:
        static constexpr int max_texture_width = 2048;
        static constexpr int max_texture_height = 2048;

        SDL_Renderer* renderer = nullptr;
        TTF_Font* sdl_font = nullptr;
        bool destroy_font = false;

        bitmap_font_info font_info;
        SDL_Color current_color = SDL_Color{ 255, 255, 255, 255 };

    public:
        simple_bitmap_font(SDL_Renderer* renderer, TTF_Font* font, unsigned char start_glyph, unsigned char end_glyph);
        simple_bitmap_font(SDL_Renderer* renderer, TTF_Font* font, const char* glyphs, size_t glyphs_size);
        simple_bitmap_font(SDL_Renderer* renderer, TTF_Font* font, const std::vector<char>& glyphs);
        virtual ~simple_bitmap_font();

        uint32_t set_color(uint32_t color);
        SDL_Color set_color(const SDL_Color& color);
        const uint32_t get_color_as_hex() const;
        const SDL_Color& get_color() const;

        void draw(
            const std::string& text,
            const SDL_Point& point,
            content_align align = content_align::top_left
        ) const;

        void draw(
            const std::string& text,
            const SDL_Rect& dstrect,
            content_align align = content_align::top_left,
            bool no_clip = true
        ) const;

        SDL_Rect measure(
            const std::string& text,
            const SDL_Point& point = SDL_Point{ 0, 0 }
        ) const;

    private:
        void create(const std::vector<char>& glyphs);

        void destroy();
    };
}

