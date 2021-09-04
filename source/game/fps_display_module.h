#pragma once
#include <isometric.h>
#include <memory>

namespace isometric::game {

    class fps_display_module : public isometric::module
    {
    private:
        double update_interval = 0.5f;
        content_align position = content_align::top_right;
        std::unique_ptr<isometric::rendering::simple_bitmap_font> bitmap_font;

    protected:
        void on_registered() override;
        void on_unregister() override;

        void on_update(double delta_time) override;
        void on_late_update(double delta_time) override;
        void on_fixed_update(double fixed_delta_time) override;

        double get_update_interval() const;
        void set_update_interval(double interval);

        content_align get_position() const;
        void set_position(content_align position);
    };

}