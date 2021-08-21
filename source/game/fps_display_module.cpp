#include "fps_display_module.h"
#include <algorithm>
#include "../assets/asset_management.h"
#include <format>

using namespace isometric;
using namespace isometric::game;
using namespace isometric::assets;
using namespace isometric::tools;

void fps_display_module::on_registered()
{
    auto asset_mgr = application::get_app()->get_asset_manager();

    auto fps_font = font::load("fps_font", "content/Enigma__2.TTF", 24);
    asset_mgr->register_asset(std::move(fps_font));
}

void fps_display_module::on_unregister()
{
    auto asset_mgr = application::get_app()->get_asset_manager();
    asset_mgr->unregister_asset("fps_font");
}

void fps_display_module::on_update(double delta_time)
{

}

void fps_display_module::on_late_update(double delta_time)
{
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

    graphics->draw_text("fps_font", std::format("FPS: {:.1f}", current_framerate), SDL_FPoint{ 5.0f, 5.0f });
}

void fps_display_module::on_fixed_update(double fixed_delta_time)
{

}