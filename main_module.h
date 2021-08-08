#pragma once
#include "module.h"
#include "image.h"
#include "camera.h"
#include "tile_map.h"
#include "world.h"

namespace isometric::game {

    class main_module : public isometric::module
    {
    private:
        std::shared_ptr<image> grasslands_image = nullptr;
        std::shared_ptr<camera> main_camera = nullptr;
        std::shared_ptr<tile_map> map = nullptr;
        std::unique_ptr<world> world = nullptr;

    protected:
        void on_registered() override;
        void on_unregister() override;

        void on_update(double delta_time) override;
        void on_late_update(double delta_time) override;
        void on_fixed_update(double fixed_delta_time) override;
    };

}