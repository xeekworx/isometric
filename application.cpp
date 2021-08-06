#include "application.h"
#include <sstream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

using namespace isometric;

bool application::application_exists = false;

application::application(const application_setup setup) : setup(setup)
{
    if (application_exists) {
        SDL_Log("Application instance already exists when creating '%s'", setup.name.c_str());
        throw std::exception("Application instance already exists");
    }
    else {
        application_exists = true;
        SDL_Log("Application [%s] constructed", setup.name.c_str());
    }
}

application::~application()
{
    SDL_Log("Application destructed");
}

const application_setup& application::get_setup() const
{
    return setup;
}

bool application::start()
{
    SDL_Log("Application [%s] starting", setup.name.c_str());

    if (!initialize()) return false;
    main_loop();

    SDL_Log("Destroying SDL renderer");
    graphics->renderer = nullptr;
    if (renderer) SDL_DestroyRenderer(renderer);

    SDL_Log("Closing window");
    if (window) SDL_DestroyWindow(window);

    SDL_Log("Shutting down SDL");
    SDL_Quit();

    SDL_Log("Application [%s] shutdown", setup.name.c_str());
    return true;
}

void application::shutdown()
{
    should_exit = true;
}

void application::main_loop()
{
    // Setup the initial beforeTime before the game loop starts up...
    // SDL_GetPerformanceCounter is in a unit of measurement only meaningful to this computer
    // SDL_GetPerformanceFrequency is a value to divide by to convert the counter to seconds
    double beforeTime = (double)SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();

    SDL_Event e = {};

    while (!should_exit) {
        while (SDL_PollEvent(&e)) {
            if (!on_event(e)) {
                shutdown();
                return;
            }
        }

        // Calculate delta time...
        // Get the current time by querying the performance counter and using the performance frequency to give it 
        // meaning (convert it to seconds)
        double currentTime = (double)SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
        float deltaTime = static_cast<float>(currentTime - beforeTime);
        beforeTime = currentTime; // Prime beforeTime for the next frame

        if (!on_render(deltaTime)) {
            SDL_Log("Rendering requested shutdown");
            shutdown();
        }
    }
}

bool application::on_render(float delta_time)
{
    graphics->clear(setup.background_color);



    SDL_RenderPresent(renderer);
    return true;
}

bool application::on_event(const SDL_Event& e)
{
    switch (e.type) {
    case SDL_QUIT:
        return false; // Returning false to end the game loop
    case SDL_KEYDOWN:
        if (setup.shutdown_on_esc && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
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
    return std::shared_ptr<isometric::graphics>();
}

bool application::initialize()
{
    SDL_Log("Application initializing");

    std::stringstream error;

    try {
        SDL_Log("SDL initializing subsystems");
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
            error << "Failed to initialize SDL: " << SDL_GetError();
            throw(error.str());
        }

        SDL_Log("SDL_image initializing");
        if (IMG_Init(IMG_INIT_PNG) == 0) {
            error << "Failed to initialize SDL_image: " << IMG_GetError();
            throw(error.str());
        }

        SDL_Log("SDL_ttf initializing");
        if (TTF_Init() < 0) {
            error << "Failed to initialize SDL_ttf: " << TTF_GetError();
            throw(error.str());
        }

        SDL_Log("Creating SDL window with the resolution %d x %d", setup.screen_width, setup.screen_height);
        if ((window = SDL_CreateWindow(
            setup.name.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            setup.screen_width, setup.screen_height,
            0
        )) == 0) {
            error << "Failed to create a window: " << SDL_GetError();
            throw(error.str());
        }

        SDL_Log("Creating SDL renderer");
        if ((renderer = SDL_CreateRenderer(window, -1, 0)) == 0) {
            error << "Failed to create the renderer: " << SDL_GetError();
            throw(std::exception(error.str().c_str()));
        }

        this->graphics = std::shared_ptr<isometric::graphics>(new isometric::graphics(renderer));

        // Allow mouse click events when clicking to focus an SDL window
        SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
    }
    catch (std::exception ex)
    {
        // This is a simple way to show a message box, if main_window failed to create this will still work since 
        // main_window will be NULL (the message box will just not have a parent):
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, setup.name.c_str(), ex.what(), window);

        // Output the error to the console, if you have one
        SDL_Log(ex.what());
        return false;
    }


    SDL_Log("Finished application initialization");
    return true;
}
