#pragma once
#include <SDL.h>
#include <memory>
#include <list>
#include "application_setup.h"
#include "../source/core/graphics.h"
#include "../source/core/input.h"
#include "../source/core/module.h"
#include "../tools/stopwatch.h"
#include "../source/tools/framerate.h"

namespace isometric {

    class application
    {
    private:
        static std::shared_ptr<application> this_app; // Singleton
        bool initialized = false;

        application_setup setup;
        bool should_exit = false;

        tools::stopwatch frame_stopwatch;          // Used to calculate delta time
        tools::stopwatch fixed_frame_stopwatch;    // Used to calculate fixed delta time

        tools::framerate current_fps;
        tools::framerate current_fixed_fps;

        SDL_Renderer* renderer = nullptr;
        SDL_Window* window = nullptr;

        std::shared_ptr<isometric::graphics> graphics = nullptr;
        std::list<std::shared_ptr<module>> modules;

    public:
        virtual ~application();

        template<class T> static std::shared_ptr<T> create(const application_setup& setup);

        bool start();
        void shutdown();
        void main_loop();

        void register_module(std::shared_ptr<module> m);
        void unregister_module(std::shared_ptr<module> m);
        void unregister_all_modules();
        const std::list<std::shared_ptr<module>>& get_modules() const;

        static std::shared_ptr<application> get_app() { return this_app; }
        const application_setup& get_setup() const;
        std::shared_ptr<isometric::graphics> get_graphics() const;
        bool is_initialized() const { return initialized; }

        static bool is_64bit();

    protected:
        application();

        /// <summary>
        /// This must be called by derived classes or modules will not have their update and late update functions 
        /// called.
        /// </summary>
        virtual void on_update(double delta_time);

        /// <summary>
        /// This must be called by derived classes or modules will not have their fixed update functions called.
        /// </summary>
        virtual void on_fixed_update(double fixed_delta_time);

        virtual bool on_event(const SDL_Event& e);
        virtual bool on_start() { return true; /* true to continue */ }
        virtual void on_shutdown() {}

    private:

        bool initialize();
        void try_call_fixed_update(double delta_time);
        void broadcast_fps(double delta_time) const;
    };

    template<class T>
    inline std::shared_ptr<T> application::create(const application_setup& setup)
    {
        if (setup.verbose_logging) SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

        if (application::this_app) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Application instance already exists when creating '%s'", setup.name.c_str());
            throw std::exception("Application instance already exists");
        }

        if (std::is_convertible<T, application>())
        {
            std::shared_ptr<T> app = std::shared_ptr<T>(new T);
            application::this_app = app;
            application::this_app->setup = setup;

            SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Application [%s] created", setup.name.c_str());

            return app;
        }
        else
        {
            throw std::exception("Failed to create application because the given type was not derived from application");
        }
    }

}