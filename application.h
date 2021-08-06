#pragma once
#include <memory>
#include <SDL.h>
#include "application_setup.h"
#include "graphics.h"

namespace isometric {

    class application
    {
    private:
        static bool application_exists;
        application_setup setup;
        bool should_exit = false;

        SDL_Renderer* renderer = nullptr;
        SDL_Window* window = nullptr;

        std::shared_ptr<isometric::graphics> graphics = nullptr;

    public:
        application(const application_setup setup);
        ~application();

        bool start();
        void shutdown();
        void main_loop();

        virtual bool on_render(float delta_time);
        virtual bool on_event(const SDL_Event& e);

        const application_setup& get_setup() const;
        std::shared_ptr<isometric::graphics> get_graphics() const;

    private:
        bool initialize();
    };

}