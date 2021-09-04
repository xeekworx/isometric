#pragma once
#include <vector>
#include <list>
#include <memory>
#include <ranges>
#include "transform.h"
#include "camera.h"
#include "tile_map.h"
#include "game_object.h"

namespace isometric {

    class world
    {
    private:
        std::vector<std::shared_ptr<camera>> cameras;
        std::list<std::shared_ptr<game_object>> objects;
        std::shared_ptr<tile_map> map;
        transform transform;
        SDL_Point selected_world_tile;

        bool update_called = false;

    public:
        world(std::shared_ptr<tile_map> map, std::shared_ptr<camera> main_camera);
        std::shared_ptr<camera> get_main_camera() const;

        void update(double delta_time);
        void render(SDL_Renderer* renderer, double delta_time);

        void set_selection(const SDL_Point& tile_point);
        const SDL_Point& get_selection() const;
        bool has_selection() const;
        void reset_selection();

        unsigned get_max_horizontal_tiles() const;
        unsigned get_max_vertical_tiles() const;

        const isometric::transform& get_transform() const
        {
            return this->transform;
        }

        void add_object(std::shared_ptr<game_object> obj);
        void remove_object(std::shared_ptr<game_object> obj);
    };

}