#include <SDL.h>
#include "input.h"

using namespace isometric;

const uint8_t * input::keyboard_state()
{
    return SDL_GetKeyboardState(NULL);
}

bool isometric::input::scancode_down(SDL_Scancode scancode)
{
    return keyboard_state()[scancode];
}

bool isometric::input::keycode_down(SDL_Keycode key)
{
    return scancode_down(SDL_GetScancodeFromKey(key));
}

SDL_FPoint input::mouse_position()
{
    int mouse_x = 0, mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    return
    {
        static_cast<float>(mouse_x),
        static_cast<float>(mouse_y),
    };
}

uint32_t input::mouse_buttons()
{
    return SDL_GetMouseState(NULL, NULL);
}

bool isometric::input::mouse_down(int button)
{
    return mouse_buttons() & SDL_BUTTON(button);
}
