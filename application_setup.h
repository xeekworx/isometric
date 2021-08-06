#pragma once
#include <string>

namespace isometric {

    struct application_setup {
        std::string name;
        bool shutdown_on_esc = true;
        int32_t background_color = 0x00000000;
        int screen_width = 1280;
        int screen_height = 720;
    };

}