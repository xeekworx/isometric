#pragma once
#include <vector>
#include <memory>
#include <ranges>
#include "transform.h"
#include "camera.h"
#include "tile_map.h"

namespace isometric {

    class world
    {
    private:
        std::vector<std::shared_ptr<camera>> cameras;
        std::shared_ptr<tile_map> map;
        transform transform;
        SDL_Point selected_world_tile;

        bool update_called = false;

    public:
        world(std::shared_ptr<tile_map> map, std::shared_ptr<camera> main_camera);
        std::shared_ptr<camera> get_main_camera() const;

        void update();
        void render(SDL_Renderer * renderer);

        void set_selection(const SDL_Point& tile_point);
        const SDL_Point& get_selection() const;
        bool has_selection() const;
        void reset_selection();

        unsigned get_max_horizontal_tiles() const;
        unsigned get_max_vertical_tiles() const;

        const isometric::transform& get_transform() const {
            return this->transform;
        }
    };

}