#pragma once

namespace isometric {

    class application;

    class input
    {
        friend application;
    private:
        input() { }

    public:

        const uint8_t * keyboard_state() const;
        bool scancode_down(SDL_Scancode scancode) const;
        bool keycode_down(SDL_Keycode key) const;

        const SDL_FPoint& mouse_position() const;
        uint32_t mouse_buttons() const;
        bool mouse_down(int button) const;
    };

}