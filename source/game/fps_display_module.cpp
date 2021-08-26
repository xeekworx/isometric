#include "fps_display_module.h"
#include <algorithm>
#include <format>
#include "../assets/asset_management.h"

using namespace isometric;
using namespace isometric::game;
using namespace isometric::assets;
using namespace isometric::tools;

void fps_display_module::on_registered()
{
    auto asset_mgr = application::get_app()->get_asset_manager();
    auto fps_font = font::load("fps_font", "content/roboto/RobotoMono-Bold.ttf", std::vector<int>{ 16, 24, 32, 48 });

    std::vector<char> glyphs = { 'F', 'P', 'S', ':', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };
    std::unordered_map<char, SDL_Surface*> surfaces;

    int x = 0, y = 0, overall_width = 0, overall_height = 0;
    for (char character : glyphs) {
        SDL_Surface* surface = TTF_RenderGlyph_Blended(fps_font->get_font(100), character, SDL_Color{ 255, 255, 255, 255 });
        if (surface) {
            if (x + surface->w >= 2048) {
                x = 0;
                y += surface->h;
            }

            surfaces[character] = surface;
            glyph_source_rects[character] = SDL_Rect{ x, y, surface->w, surface->h };

            x += surface->w;

            if (x > overall_width) overall_width = x;
            if (y + surface->h > overall_height) overall_height = y + surface->h;
        }
    }

    SDL_Surface* bitmap_font_surface = SDL_CreateRGBSurface(0, overall_width, overall_height, 32,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
        0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif
    );

    SDL_FillRect(bitmap_font_surface, NULL, SDL_MapRGBA(bitmap_font_surface->format, 255, 255, 255, 0));

    for (auto &pair : surfaces) {
        SDL_Rect srcrect{ 0, 0, glyph_source_rects[pair.first].w, glyph_source_rects[pair.first].h };
        SDL_Rect dstrect = glyph_source_rects[pair.first];

        SDL_SetSurfaceBlendMode(pair.second, SDL_BLENDMODE_BLEND);
        SDL_SetSurfaceBlendMode(bitmap_font_surface, SDL_BLENDMODE_BLEND);
        SDL_BlitSurface(pair.second, &srcrect, bitmap_font_surface, &dstrect);
        SDL_FreeSurface(pair.second);
    }

    //IMG_SavePNG(bitmap_font_surface, "glyphs.png");
    auto graphics = application::get_app()->get_graphics();
    if (graphics && graphics->get_renderer()) {
        glyphs_texture = SDL_CreateTextureFromSurface(graphics->get_renderer(), bitmap_font_surface);
        SDL_SetTextureBlendMode(glyphs_texture, SDL_BLENDMODE_ADD);
    }
    SDL_FreeSurface(bitmap_font_surface);

    asset_mgr->register_asset(std::move(fps_font));
}

void fps_display_module::on_unregister()
{
    auto asset_mgr = application::get_app()->get_asset_manager();
    asset_mgr->unregister_asset("fps_font");

    if (glyphs_texture) {
        SDL_DestroyTexture(glyphs_texture);
        glyphs_texture = nullptr;
    }
}

void fps_display_module::on_update(double delta_time)
{

}

static void draw_fps(SDL_Renderer* renderer, SDL_Texture* glyphs_texture, const std::unordered_map<char, SDL_Rect>& glyph_source_rects, const SDL_Point& point, const std::string& text)
{
    SDL_Rect dstrect = { point.x, 0, 0, 0 };

    SDL_SetTextureColorMod(glyphs_texture, 0, 255, 255);

    for (char character : text) {
        if (!glyph_source_rects.contains(character)) continue;
        const SDL_Rect& srcrect = glyph_source_rects.at(character);
        dstrect = { dstrect.x, point.y, srcrect.w, srcrect.h };
        SDL_RenderCopy(renderer, glyphs_texture, &srcrect, &dstrect);
        dstrect.x += srcrect.w;
    }
}

void fps_display_module::on_late_update(double delta_time)
{
    SDL_Rect viewport = application::get_app()->get_viewport();
    auto graphics = application::get_app()->get_graphics();
    const auto& framerate = application::get_app()->get_framerate();
    if (!graphics) return;

    static double current_framerate = framerate.get();
    static double elapsed_since_last_update = 0.0;
    elapsed_since_last_update += delta_time;

    if (elapsed_since_last_update >= 0.5) {
        elapsed_since_last_update = 0.0;
        current_framerate = framerate.get();
    }

    constexpr int margin = 6;
    viewport.x += margin;
    viewport.y += margin;
    viewport.w -= margin * 2;
    viewport.h -= margin * 2;

    graphics->set_color(0xFFFFFFFF);
    //graphics->draw_text("fps_font", 100, std::format("FPS: {:.1f}", current_framerate), viewport, text_align_top | text_align_right);
    draw_fps(graphics->get_renderer(), glyphs_texture, glyph_source_rects, SDL_Point{ 6, 6 }, std::format("FPS: {:.1f}", current_framerate));
}

void fps_display_module::on_fixed_update(double fixed_delta_time)
{

}