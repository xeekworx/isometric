#include <SDL.h>
#include "input.h"

using namespace isometric;

const uint8_t * input::keyboard_state() const
{
    return SDL_GetKeyboardState(NULL);
}

bool isometric::input::scancode_down(SDL_Scancode scancode) const
{
    return keyboard_state()[scancode];
}

bool isometric::input::keycode_down(SDL_Keycode key) const
{
    return scancode_down(SDL_GetScancodeFromKey(key));
}

const SDL_FPoint& input::mouse_position() const
{
    static SDL_FPoint current_position = {};

    int x = 0, y = 0;
    SDL_GetMouseState(&x, &x);

    current_position.x = x;
    current_position.y = y;

    return current_position;
}

uint32_t input::mouse_buttons() const
{
    return SDL_GetMouseState(NULL, NULL);
}

bool isometric::input::mouse_down(int button) const
{
    return mouse_buttons() & SDL_BUTTON(button);
}
