#include "simple_bitmap_font.h"
#include <SDL_ttf.h>
#include <vector>
#include <tuple>

using namespace isometric::rendering;

static void generate_glyph_surfaces(TTF_Font* font, const std::vector<char>& glyphs, bitmap_font_info& font_info);
static void generate_glyph_srcrects(bitmap_font_info& font_info);
static void generate_glyph_textures(SDL_Renderer* renderer, bitmap_font_info& font_info, bool free_glyph_surfaces = true);

simple_bitmap_font::simple_bitmap_font(SDL_Renderer* renderer, TTF_Font* font, unsigned char start_glyph, unsigned char end_glyph)
    : renderer(renderer), sdl_font(font)
{
    size_t num_glyphs = (static_cast<size_t>(end_glyph) - start_glyph) + 1; // end_glyph is inclusive so + 1
    std::vector<char> glyphs(num_glyphs);

    unsigned char current_glyph = start_glyph;
    for (size_t i = 0; i < num_glyphs; current_glyph++, i++)
    {
        glyphs[i] = static_cast<char>(current_glyph);
    }

    create(glyphs);
}

simple_bitmap_font::simple_bitmap_font(SDL_Renderer* renderer, TTF_Font* font, const char* glyphs, size_t glyphs_size)
    : simple_bitmap_font(renderer, font, std::vector<char>(glyphs, glyphs + glyphs_size))
{

}

simple_bitmap_font::simple_bitmap_font(SDL_Renderer* renderer, TTF_Font* font, const std::vector<char>& glyphs)
    : renderer(renderer), sdl_font(font)
{
    create(glyphs);
}

simple_bitmap_font::~simple_bitmap_font()
{
    destroy();
}

uint32_t simple_bitmap_font::set_color(uint32_t color)
{
    auto pixel_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

    SDL_Color new_color{};
    SDL_GetRGBA(
        color, pixel_format,
        &new_color.r,
        &new_color.g,
        &new_color.b,
        &new_color.a
    );

    uint32_t old_color = get_color_as_hex();
    set_color(new_color);

    SDL_FreeFormat(pixel_format);

    return old_color;
}

SDL_Color simple_bitmap_font::set_color(const SDL_Color& color)
{
    SDL_Color old_color = current_color;
    current_color = color;
    return old_color;
}

const uint32_t simple_bitmap_font::get_color_as_hex() const
{
    SDL_Color color = get_color();

    return
        color.a +
        (color.b << 8) +
        (color.g << 16) +
        (color.r << 24);
}

const SDL_Color& simple_bitmap_font::get_color() const
{
    return current_color;
}

void simple_bitmap_font::draw(
    const std::string& text,
    const SDL_Point& point,
    content_align align
) const
{
    SDL_Rect dstrect = { point.x, point.y, 0, 0 };

    if (align != content_align::top_left)
    {
        auto measured_rect = measure(text, SDL_Point{ dstrect.x, dstrect.y });

        // The alignment has to be handled here since it works differntly than when using the dstrect version of this
        // function.

        // Horizontal Alignment:
        switch (align)
        {
        case content_align::top_center:
        case content_align::middle_center:
        case content_align::bottom_center:
            dstrect.x -= static_cast<int>(std::round(measured_rect.w / 2.0));
            break;
        case content_align::top_right:
        case content_align::middle_right:
        case content_align::bottom_right:
            dstrect.x -= measured_rect.w;
            break;
        }

        // Vertical Alignment:
        switch (align)
        {
        case content_align::middle_left:
        case content_align::middle_center:
        case content_align::middle_right:
            dstrect.y -= static_cast<int>(std::round(measured_rect.h / 2.0));
            break;
        case content_align::bottom_left:
        case content_align::bottom_center:
        case content_align::bottom_right:
            dstrect.y -= measured_rect.h;
            break;
        }
    }

    draw(text, dstrect, content_align::top_left, true);
}

void simple_bitmap_font::draw(
    const std::string& text,
    const SDL_Rect& dstrect,
    content_align align,
    bool no_clip
) const
{
    for (const auto& texture_info : font_info.textures)
    {
        SDL_Texture* texture = std::get<0>(texture_info);
        SDL_SetTextureColorMod(texture, current_color.r, current_color.g, current_color.b);
        SDL_SetTextureAlphaMod(texture, current_color.a);
    }

    // To keep up with the current glyph drawing position:
    SDL_Rect glyph_dstrect{ dstrect.x, dstrect.y };

    if (align != content_align::top_left)
    {
        auto measured_rect = measure(text, SDL_Point{ dstrect.x, dstrect.y });

        // Horizontal Alignment:
        switch (align)
        {
        case content_align::top_center:
        case content_align::middle_center:
        case content_align::bottom_center:
            glyph_dstrect.x += static_cast<int>(std::round(dstrect.w / 2.0 - measured_rect.w / 2.0));
            break;
        case content_align::top_right:
        case content_align::middle_right:
        case content_align::bottom_right:
            glyph_dstrect.x += dstrect.w - measured_rect.w;
            break;
        }

        // Vertical Alignment:
        switch (align)
        {
        case content_align::middle_left:
        case content_align::middle_center:
        case content_align::middle_right:
            glyph_dstrect.y += static_cast<int>(std::round(dstrect.h / 2.0 - measured_rect.h / 2.0));
            break;
        case content_align::bottom_left:
        case content_align::bottom_center:
        case content_align::bottom_right:
            glyph_dstrect.y += dstrect.h - measured_rect.h;
            break;
        }
    }

    // Text rendering:
    for (char character : text)
    {
        if (!font_info.glyphs.contains(character)) continue;

        const auto& info = font_info.glyphs.at(character);
        glyph_dstrect = { glyph_dstrect.x, glyph_dstrect.y, info.srcrect.w, info.srcrect.h };

        if (!no_clip && !SDL_HasIntersection(&glyph_dstrect, &dstrect))
        {
            break;
        }

        if (info.texture_index < font_info.textures.size())
        {
            SDL_Texture* texture = std::get<0>(font_info.textures[info.texture_index]);
            if (texture)
            {
                SDL_RenderCopy(renderer, texture, &info.srcrect, &glyph_dstrect);
            }
        }

        glyph_dstrect.x += info.srcrect.w;
    }
}

SDL_Rect simple_bitmap_font::measure(
    const std::string& text,
    const SDL_Point& point
) const
{
    int width = 0, height = 0;

    for (char character : text)
    {
        if (font_info.glyphs.contains(character))
        {
            const auto& info = font_info.glyphs.at(character);
            width += info.srcrect.w;
            if (info.srcrect.h > height)
            {
                height = info.srcrect.h;
            }
        }
    }

    return SDL_Rect{ point.x, point.y, width, height };
}

void simple_bitmap_font::create(const std::vector<char>& glyphs)
{
    generate_glyph_surfaces(sdl_font, glyphs, font_info);
    generate_glyph_srcrects(font_info);
    generate_glyph_textures(renderer, font_info, true);
}

void simple_bitmap_font::destroy()
{
    for (auto& texture_info : font_info.textures)
    {
        auto& texture = std::get<0>(texture_info);
        if (texture)
        {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }

    font_info.textures.clear();
    font_info.glyphs.clear();

    if (destroy_font && this->sdl_font)
    {
        TTF_CloseFont(this->sdl_font);
        this->sdl_font = nullptr;
    }
}

static void generate_glyph_surfaces(
    TTF_Font* font,
    const std::vector<char>& glyphs,
    bitmap_font_info& font_info
)
{
    if (!font || glyphs.empty()) return;
    font_info.glyphs.clear();

    for (char character : glyphs)
    {
        SDL_Surface* surface = TTF_RenderGlyph_Blended(font, character, SDL_Color{ 255, 255, 255, 255 });
        if (surface)
        {
            font_info.glyphs[character] = glyph_info{
                surface,
                SDL_Rect{ 0, 0, surface->w, surface->h },
                0
            };
        }
    }
}

static void generate_glyph_srcrects(
    bitmap_font_info& font_info
)
{
    if (!font_info.textures.empty() || font_info.glyphs.empty()) return;

    constexpr int max_texture_width = 2048, max_texture_height = 2048;
    int texture_width = 0, texture_height = 0;
    size_t texture_index = 0;
    int x = 0, y = 0, row_height = 0;

    for (auto& pair : font_info.glyphs)
    {
        char character = pair.first;
        glyph_info& glyph = pair.second;

        // The current row's height should be the tallest glyph:
        if (glyph.surface->h > row_height)
            row_height = glyph.surface->h;

        // If the farthest x-extent has been reached, start a new row:
        if (x + glyph.surface->w >= max_texture_width)
        {
            x = 0;
            y += row_height;
            row_height = glyph.surface->h;
        }

        // If the farthest y-extent has been reached, configure the texture's dimensions and move onto the next 
        // texture:
        if (y + glyph.surface->h >= max_texture_height)
        {
            font_info.textures.push_back(
                std::make_tuple<SDL_Texture*, SDL_Rect>(
                    nullptr,
                    SDL_Rect{ 0, 0, texture_width, texture_height }
                )
            );

            texture_index++;
            texture_width = texture_height = x = y = 0;
        }

        // Set the glyph's source rectangle:
        glyph.srcrect = SDL_Rect{ x, y, glyph.surface->w, glyph.surface->h };
        glyph.texture_index = texture_index;

        // Increment the x coord:
        x += glyph.surface->w;

        // Use the greatest x or y to update the texture_width and texture_height:
        if (x > texture_width) texture_width = x; // Not adding the glyph width since it was done above
        if (y + row_height > texture_height) texture_height = y + row_height;
    }

    // The last texture's dimension needs to be setup after the loop:
    if (texture_width > 0 && texture_height > 0)
    {
        font_info.textures.push_back(
            std::make_tuple<SDL_Texture*, SDL_Rect>(
                nullptr,
                SDL_Rect{ 0, 0, texture_width, texture_height }
            )
        );
    }
}

static void generate_glyph_textures(
    SDL_Renderer* renderer,
    bitmap_font_info& font_info,
    bool free_glyph_surfaces
)
{
    std::vector<SDL_Surface*> atlas_surfaces;
    for (auto& texture_info : font_info.textures)
    {
        const SDL_Rect& dimensions = std::get<1>(texture_info);

        SDL_Surface* atlas_surface = SDL_CreateRGBSurface(0, dimensions.w, dimensions.h, 32,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
            0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif
        );

        atlas_surfaces.push_back(atlas_surface);
    }

    for (auto& pair : font_info.glyphs)
    {
        char character = pair.first;
        glyph_info& glyph = pair.second;
        const size_t texture_index = glyph.texture_index;

        // This is the dstrect in this context, since the destination 
        // surface will be the source in the future
        SDL_Rect dstrect = glyph.srcrect;
        SDL_Rect srcrect = SDL_Rect{ 0, 0, dstrect.w, dstrect.h };
        SDL_Surface* dst = atlas_surfaces[texture_index];
        if (dst && glyph.surface)
        {
            // Disable blending for the glyph surface because if blending is enabled, black will bleed through the 
            // anti-aliasing even though it's 0,0,0,0
            SDL_SetSurfaceBlendMode(glyph.surface, SDL_BLENDMODE_NONE);
            SDL_BlitSurface(glyph.surface, &srcrect, dst, &dstrect);
            if (free_glyph_surfaces)
            {
                SDL_FreeSurface(glyph.surface);
                glyph.surface = nullptr;
            }
        }
    }

    for (size_t texture_index = 0; texture_index < font_info.textures.size(); texture_index++)
    {
        auto& texture = std::get<0>(font_info.textures[texture_index]);
        auto& surface = atlas_surfaces[texture_index];

        if (surface)
        {
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            // For testing, output the glyph atlas as an image file. Don't forget to #include <SDL_image.h>
            // IMG_SavePNG(surface, std::format("./simple_font.{}.png", texture_index).c_str());
            SDL_FreeSurface(surface);
            surface = nullptr;
        }
    }
}