#include "fps_display_module.h"
#include <algorithm>
#include <format>

using namespace isometric;
using namespace isometric::game;
using namespace isometric::assets;
using namespace isometric::tools;
using namespace isometric::rendering;

void fps_display_module::on_registered()
{
    auto asset_mgr = application::get_app()->get_asset_manager();
    auto fps_font = font::load("fps_font", "content/roboto/RobotoMono-Bold.ttf", std::vector<int>{ 16, 21, 32 });

    std::vector<char> glyphs = { 'F', 'P', 'S', ':', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ' ', '\t' };

    bitmap_font = std::make_unique<simple_bitmap_font>(
        application::get_app()->get_renderer(),
        fps_font->get_font(21),
        glyphs.data(), glyphs.size()
    );

    asset_mgr->register_asset(std::move(fps_font));
}

void fps_display_module::on_unregister()
{
    if (bitmap_font) bitmap_font.reset();

    auto app = application::get_app();
    if (app)
    {
        auto asset_mgr = app->get_asset_manager();
        if (asset_mgr) asset_mgr->unregister_asset("fps_font");
    }
}

void fps_display_module::on_update(double delta_time)
{

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

    if (elapsed_since_last_update >= update_interval)
    {
        elapsed_since_last_update = 0.0;
        current_framerate = framerate.get();
    }

    constexpr int margin = 6;
    viewport.x += margin;
    viewport.y += margin;
    viewport.w -= margin * 2;
    viewport.h -= margin * 2;

    // Render using bitmap font:
    bitmap_font->set_color(0xFFFFFFFF);
    bitmap_font->draw(
        std::format("FPS: {:.1f}", current_framerate),
        viewport,
        position
    );

    // Render using what graphics uses (SDL_ttf):
    /*
    graphics->set_color(0xFFFFFFFF);
    graphics->draw_text(
        "fps_font", 21,
        std::format("FPS: {:.1f}", current_framerate),
        viewport,
        content_align::top_left
    );
    */
}

void fps_display_module::on_fixed_update(double fixed_delta_time)
{

}

double fps_display_module::get_update_interval() const
{
    return update_interval;
}

void isometric::game::fps_display_module::set_update_interval(double interval)
{
    update_interval = interval;
}

content_align fps_display_module::get_position() const
{
    return position;
}

void fps_display_module::set_position(content_align position)
{
    this->position = position;
}
