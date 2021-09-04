#pragma once
#include <isometric.h>
#include <memory>
#include "../graphics/simple_bitmap_font.h"

namespace isometric::game {

    enum class fps_display_position {
        top_left, top_right, top_center, bottom_left, bottom_right, bottom_center
    };

    class fps_display_module : public isometric::module
    {
    private:
        double update_interval = 0.5f;
        fps_display_position position = fps_display_position::top_right;
        std::unique_ptr<isometric::bitmap_font::simple_bitmap_font> bitmap_font;

    protected:
        void on_registered() override;
        void on_unregister() override;

        void on_update(double delta_time) override;
        void on_late_update(double delta_time) override;
        void on_fixed_update(double fixed_delta_time) override;

        double get_update_interval() const;
        void set_update_interval(double interval);

        fps_display_position get_position() const;
        void set_position(fps_display_position position);
    };

}