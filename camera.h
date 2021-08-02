#pragma once
#include <memory>
#include <SDL.h>

namespace isometric {

    class camera
    {
    private:
        unsigned viewport_width = 0;
        unsigned viewport_height = 0;
        unsigned viewport_x = 0;
        unsigned viewport_y = 0;
        float current_tile_x = 0;
        float current_tile_y = 0;
        bool enabled = true;

        camera() {}

    public:
        static std::shared_ptr<camera> create(unsigned viewport_x, unsigned viewport_y, unsigned width, unsigned height, float start_tile_x = 0, float start_tile_y = 0);

        void enable(bool enable = true);
        void disable();
        bool is_enabled() const;

        unsigned get_viewport_x() const;
        unsigned get_viewport_y() const;
        void set_viewport(unsigned x, unsigned y, unsigned width = 0, unsigned height = 0);

        unsigned get_width() const;
        unsigned get_height() const;

        void set_current_pos(float tile_x, float tile_y);
        void set_current_pos(const SDL_FPoint& point);

        float get_current_x() const;
        void set_current_x(float tile_x);

        float get_current_y() const;
        void set_current_y(float tile_y);
    };
}