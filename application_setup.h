#pragma once
#include <string>

namespace isometric {

    struct application_setup {
        std::string name;

        bool shutdown_on_esc = true;
        bool mouse_focus_clickthrough = false;
        bool verbose_logging = false;
        int32_t background_color = 0x00000000;
        int screen_width = 1280;
        int screen_height = 720;
        bool vertical_sync = false;
        double fixed_update_fps = 50.0;

        bool broadcast_fps = false;
        float broadcast_fps_elapsed = 5.0F;
    };

}