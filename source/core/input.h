#pragma once

namespace isometric {

    class application;

    class input
    {
    private:
        input() { } // Force static class

    public:

        static const uint8_t * keyboard_state();
        static bool scancode_down(SDL_Scancode scancode);
        static bool keycode_down(SDL_Keycode key);

        static SDL_FPoint mouse_position();
        static uint32_t mouse_buttons();
        static bool mouse_down(int button);
    };

}