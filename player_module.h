#pragma once
#include "module.h"
#include "image.h"
#include "camera.h"
#include "tile_map.h"
#include "world.h"

namespace isometric::game {

    class player_module : public isometric::module
    {
    private:
        std::shared_ptr<tile_map> map = nullptr;
        std::shared_ptr<world> world = nullptr;
        SDL_FPoint location;

    protected:
        void on_registered() override;
        void on_unregister() override;

        void on_update(double delta_time) override;
        void on_late_update(double delta_time) override;
        void on_fixed_update(double fixed_delta_time) override;

    public:
        void setup(std::shared_ptr<tile_map> map, std::shared_ptr<isometric::world> world);
        void set_player_location(const SDL_FPoint& point);
        const SDL_FPoint& get_player_location() const;
    };

}