#include "application.h"
#include <sstream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

using namespace isometric;
using namespace isometric::assets;
using namespace isometric::rendering;

std::shared_ptr<application> application::this_app = nullptr;

application::application()
{
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Application constructed");
}

application::~application()
{
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Application destructed");
}

const application_setup& application::get_setup() const
{
    return setup;
}

SDL_Rect application::get_viewport() const
{
    SDL_Rect viewport{ 0 };
    SDL_RenderGetViewport(renderer, &viewport);
    return viewport;
}

SDL_FRect isometric::application::get_viewportf() const
{
    SDL_Rect viewport = get_viewport();
    return SDL_FRect{
        static_cast<float>(viewport.x),
        static_cast<float>(viewport.y),
        static_cast<float>(viewport.w),
        static_cast<float>(viewport.h)
    };
}

SDL_Renderer* application::get_renderer() const
{
    return renderer;
}

bool application::start()
{
    SDL_Log("Application [%s] starting", setup.name.c_str());

    if (initialize())
    {
        if (!on_start())
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Application's [%s] on_start returned false", setup.name.c_str());
        }
        else
        {
            main_loop();
        }
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Calling shutdown event");
    on_shutdown();

    if (setup.broadcast_fps)
    {
        SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Minimum FPS: %.02f, Maximum FPS: %0.2f, Average FPS: %0.2f",
            current_fps.get_minimum(), current_fps.get_maximum(), current_fps.get_overall_average());
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Unregistering %llu modules", modules.size());
    unregister_all_modules();

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Destroying asset manager");
    asset_manager->shutdown();

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Destroying SDL renderer");
    graphics->renderer = nullptr;
    if (renderer) SDL_DestroyRenderer(renderer);

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Closing window");
    if (window) SDL_DestroyWindow(window);

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Shutting down SDL");
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    SDL_Log("Application [%s] shutdown", setup.name.c_str());
    return true;
}

void application::shutdown()
{
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "application::shutdown called > should_exit before call = %s",
        should_exit ? "true" : "false");

    should_exit = true;
}

void application::main_loop()
{
    SDL_Event e = {};

    // Setup the initial beforeTime before the game loop starts up...
    //double before_time = static_cast<double>(SDL_GetPerformanceCounter()) / SDL_GetPerformanceFrequency();
    frame_stopwatch.start(true);
    fixed_frame_stopwatch.start(true);

    while (!should_exit)
    {
        while (SDL_PollEvent(&e))
        {
            if (!on_event(e))
            {
                shutdown();
                return;
            }
        }

        // Calculate delta time...
        frame_stopwatch.stop();
        double delta_time = frame_stopwatch.get_elapsed_sec();
        frame_stopwatch.restart();

        // Calculate framerate...
        // Fixed framerate is determined by try_call_fixed_udpate() later
        current_fps.set_from_delta(delta_time);

        graphics->clear(setup.background_color);

        try_call_fixed_update(delta_time);
        on_update(delta_time);

        // If you get the following error in the log or console from SDL it means there wasn't enough drawn to enable
        // SDL's internal batching. This error can be ignored.
        // ERROR: SDL failed to get a vertex buffer for this Direct3D 9 rendering batch!
        graphics->present();

        if (setup.broadcast_fps) broadcast_fps(delta_time);
    }
}

void application::broadcast_fps(double delta_time) const
{
    static double time_since_last_update = 0.0;
    time_since_last_update += delta_time;

    if (time_since_last_update >= setup.broadcast_fps_elapsed /* seconds */)
    {
        time_since_last_update = 0.0;

        SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Current FPS: %.02f, Current Fixed FPS: %0.2f",
            current_fps.get(), current_fixed_fps.get());

        SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Average FPS: %.02f, Average Fixed FPS: %0.2f",
            current_fps.get_average(), current_fixed_fps.get_average());
    }
}

void application::try_call_fixed_update(double delta_time)
{
    constexpr int max_steps = 5; // Maximum number of steps, to avoid degrading to an halt.
    static double fixed_timestep = 1.0 / setup.fixed_update_fps;
    static double fixed_update_accumulator = 0.0;
    static double fixed_update_accumulator_ratio = 0.0;

    fixed_update_accumulator += delta_time;
    const int steps = static_cast<int>(std::floor(fixed_update_accumulator / fixed_timestep));

    // To avoid rounding errors, touches fixed_update_accumulator only if needed.
    if (steps > 0)
    {
        fixed_update_accumulator -= steps * fixed_timestep;
    }

    fixed_update_accumulator_ratio = fixed_update_accumulator / fixed_timestep;

    // This is similar to clamp "dt":  dt = std::min (dt, MAX_STEPS * FIXED_TIMESTEP)
    // but it allows above calculations of fixedTimestepAccumulator_ and fixed_update_accumulator_ratio to remain 
    // unchanged.
    const int steps_clamped = std::min(steps, max_steps);
    for (int i = 0; i < steps_clamped; i++)
    {
        // Calculate fixed delta time:
        fixed_frame_stopwatch.stop();
        double fixed_delta_time = fixed_frame_stopwatch.get_elapsed_sec();
        fixed_frame_stopwatch.restart();

        current_fixed_fps.set_from_delta(fixed_delta_time);
        on_fixed_update(fixed_delta_time);
    }
}

void application::on_fixed_update(double fixed_delta_time)
{
    for (auto& m : modules)
    {
        if (m && m->is_enabled()) m->on_fixed_update(fixed_delta_time);
    }
}

void application::on_update(double delta_time)
{
    for (auto& m : modules)
    {
        if (m && m->is_enabled()) m->on_update(delta_time);
    }

    for (auto& m : modules)
    {
        if (m && m->is_enabled()) m->on_late_update(delta_time);
    }
}

bool application::on_event(const SDL_Event& e)
{
    static size_t mouse_motion = 0;

    switch (e.type)
    {
    case SDL_QUIT:
        return false; // Returning false to end the game loop
    case SDL_KEYDOWN:
        if (setup.shutdown_on_esc && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
            SDL_Log("Shutdown requested via escape key");
            return false; // Shutdown
        }
        break;
    }

    // Keep the game loop running by returning true
    return true;
}

std::shared_ptr<graphics> application::get_graphics() const
{
    return graphics;
}

std::shared_ptr<isometric::assets::asset_management> isometric::application::get_asset_manager() const
{
    return asset_manager;
}

bool application::initialize()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Application initializing");

    SDL_Log("Current platform: %s", SDL_GetPlatform());
    SDL_Log("Application architecture: %d-Bit", is_64bit() ? 64 : 32);

    SDL_version sdlvn_built = {}, sdlvn_runtime = {};
    SDL_VERSION(&sdlvn_built);
    SDL_GetVersion(&sdlvn_runtime);
    SDL_Log("Built with SDL version: %d.%d.%d, Using SDL runtime version: %d.%d.%d",
        sdlvn_built.major, sdlvn_built.minor, sdlvn_built.patch,
        sdlvn_runtime.major, sdlvn_runtime.minor, sdlvn_runtime.patch
    );

    std::stringstream error;

    try
    {
        // --------------------------------------------------------------------
        // SDL & EXTENSIONS INIT

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SDL initializing subsystems");
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            error << "Failed to initialize SDL: " << SDL_GetError();
            throw(error.str());
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SDL_image initializing");
        if (IMG_Init(IMG_INIT_PNG) == 0)
        {
            error << "Failed to initialize SDL_image: " << IMG_GetError();
            throw(error.str());
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf initializing");
        if (TTF_Init() < 0)
        {
            error << "Failed to initialize SDL_ttf: " << TTF_GetError();
            throw(error.str());
        }

        // --------------------------------------------------------------------
        // SDL HINTS

        // Allow mouse click events when clicking to focus an SDL window
        SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, setup.mouse_focus_clickthrough ? "1" : "0");

        // --------------------------------------------------------------------
        // SDL_WINDOW

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Creating SDL window with the resolution %d x %d", setup.screen_width, setup.screen_height);
        if ((window = SDL_CreateWindow(
            setup.name.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            setup.screen_width, setup.screen_height,
            0
        )) == 0)
        {
            error << "Failed to create a window: " << SDL_GetError();
            throw(error.str());
        }

        // --------------------------------------------------------------------
        // SDL_RENDERER, ASSET MANAGER, & GRAPHICS

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Creating SDL renderer");
        Uint32 renderer_flags = setup.vertical_sync ? SDL_RENDERER_PRESENTVSYNC : 0;
        if ((renderer = SDL_CreateRenderer(window, -1, renderer_flags)) == 0)
        {
            error << "Failed to create the renderer: " << SDL_GetError();
            throw(std::exception(error.str().c_str()));
        }

        this->asset_manager = std::shared_ptr<asset_management>(new asset_management(renderer));
        this->graphics = std::shared_ptr<rendering::graphics>(new rendering::graphics(renderer));

    }
    catch (std::exception ex)
    {
        // This is a simple way to show a message box, if main_window failed to create this will still work since 
        // main_window will be NULL (the message box will just not have a parent):
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, setup.name.c_str(), ex.what(), window);

        // Output the error to the console, if you have one
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, ex.what());
        return false;
    }

    initialized = true;
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Finished application initialization");
    return initialized;
}

bool application::is_64bit()
{
#if UINTPTR_MAX == UINT32_MAX
    return false;
#elif UINTPTR_MAX == UINT64_MAX
    return true;
#else
    return false;
#endif
}

void application::register_module(std::shared_ptr<module> m)
{
    modules.push_back(m);
    m->app = application::this_app;

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Module [%s] registered with application [%s]",
        m->name.c_str(), setup.name.c_str());

    m->on_registered();
}

void application::unregister_module(std::shared_ptr<module> m)
{
    m->on_unregister();

    m->app = nullptr;
    modules.remove(m);

    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Module [%s] unregistered with application [%s]",
        m->name.c_str(), setup.name.c_str());
}

void application::unregister_all_modules()
{
    auto currente_modules = std::list<std::shared_ptr<module>>(modules.begin(), modules.end());

    for (auto m : currente_modules)
    {
        unregister_module(m);
    }
}

const std::list<std::shared_ptr<module>>& application::get_modules() const
{
    return modules;
}
