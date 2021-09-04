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
    if (app) {
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

    if (elapsed_since_last_update >= update_interval) {
        elapsed_since_last_update = 0.0;
        current_framerate = framerate.get();
    }

    constexpr int margin = 6;
    viewport.x += margin;
    viewport.y += margin;
    viewport.w -= margin * 2;
    viewport.h -= margin * 2;

    simple_bitmap_font::text_valign valign = simple_bitmap_font::text_valign::top;
    simple_bitmap_font::text_halign halign = simple_bitmap_font::text_halign::left;
    switch (position) {
    default:
    case fps_display_position::top_left:
        valign = simple_bitmap_font::text_valign::top;
        halign = simple_bitmap_font::text_halign::left;
        break;
    case fps_display_position::top_right:
        valign = simple_bitmap_font::text_valign::top;
        halign = simple_bitmap_font::text_halign::right;
        break;
    case fps_display_position::top_center:
        valign = simple_bitmap_font::text_valign::top;
        halign = simple_bitmap_font::text_halign::center;
        break;
    case fps_display_position::bottom_left:
        valign = simple_bitmap_font::text_valign::bottom;
        halign = simple_bitmap_font::text_halign::left;
        break;
    case fps_display_position::bottom_right:
        valign = simple_bitmap_font::text_valign::bottom;
        halign = simple_bitmap_font::text_halign::right;
        break;
    case fps_display_position::bottom_center:
        valign = simple_bitmap_font::text_valign::bottom;
        halign = simple_bitmap_font::text_halign::center;
        break;
    }

    bitmap_font->set_color(0xFFFFFFFF);
    bitmap_font->draw(std::format(
        "FPS: {:.1f}", current_framerate),
        viewport,
        valign,
        halign
    );
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

fps_display_position fps_display_module::get_position() const
{
    return position;
}

void fps_display_module::set_position(fps_display_position position)
{
    this->position = position;
}
