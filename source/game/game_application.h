#pragma once
#include <isometric.h>
#include "camera_module.h"
#include "player_module.h"
#include "fps_display_module.h"

namespace isometric::game {

    class game_application : public isometric::application
    {
    private:
        // TODO: Convert this to unique_ptr and move to asset management
        std::shared_ptr<isometric::assets::image> grasslands_image = nullptr;
        std::shared_ptr<camera> main_camera = nullptr;
        std::shared_ptr<tile_map> map = nullptr;
        std::shared_ptr<world> world = nullptr;

        std::shared_ptr<camera_module> camera_module;
        std::shared_ptr<player_module> player_module;
        std::shared_ptr<fps_display_module> fps_display_module;

        bool load_map();

    protected:
        bool on_start() override;
        void on_update(double delta_time) override;
        void on_fixed_update(double fixed_delta_time) override;
    };

}