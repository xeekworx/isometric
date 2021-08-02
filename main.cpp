#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <format>

#include "world.h"

int main(int argc, char* argv[])
{
    std::string app_title = "SDL Isometric Lab";
    int screen_width = 1920, screen_height = 1080;
    int tile_width = 64, tile_height = 32;
    SDL_Window* main_window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event = { 0 };
    bool should_quit = false;
    std::stringstream error;
    float current_fps = -0.0;
    float time_since_last_fps = 0.0;

    SDL_Rect grass1_tile_srcrect = { 0, 0, tile_width, tile_height };
    SDL_Rect grass2_tile_srcrect = { 64, 0, tile_width, tile_height };
    SDL_Rect grass3_tile_srcrect = { 128, 0, tile_width, tile_height };
    SDL_Rect grass4_tile_srcrect = { 192, 0, tile_width, tile_height };
    SDL_Rect grass5_tile_srcrect = { 256, 0, tile_width, tile_height };
    SDL_Rect grass6_tile_srcrect = { 320, 0, tile_width, tile_height };
    SDL_Rect grass7_tile_srcrect = { 384, 0, tile_width, tile_height };
    SDL_Rect grass8_tile_srcrect = { 448, 0, tile_width, tile_height };
    SDL_Rect grass9_tile_srcrect = { 512, 0, tile_width, tile_height };
    SDL_Rect grass10_tile_srcrect = { 576, 0, tile_width, tile_height };
    SDL_Rect grass11_tile_srcrect = { 640, 0, tile_width, tile_height };
    SDL_Rect grass12_tile_srcrect = { 704, 0, tile_width, tile_height };
    SDL_Rect grass13_tile_srcrect = { 768, 0, tile_width, tile_height };
    SDL_Rect grass14_tile_srcrect = { 832, 0, tile_width, tile_height };
    SDL_Rect grass15_tile_srcrect = { 896, 0, tile_width, tile_height };
    SDL_Rect grass16_tile_srcrect = { 960, 0, tile_width, tile_height };

    SDL_Rect bush1_tile_srcrect = { 512, 320, tile_width, tile_height * 2 };
    SDL_Rect bush2_tile_srcrect = { 576, 320, tile_width, tile_height * 2 };

    SDL_Rect tree1_tile_srcrect = { 256, 1152, tile_width * 2, tile_height * 6 };

    srand((unsigned)time(NULL));

    try {
        // INITIALIZATION:
        // ------------------------------------------------------------------------------------------------------------
        // Initialize SDL and create a window and renderer

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
            error << "Failed to initialize SDL: " << SDL_GetError();
            throw(error.str());
        }

        if (IMG_Init(IMG_INIT_PNG) == 0) {
            error << "Failed to initialize SDL_image: " << IMG_GetError();
            throw(error.str());
        }

        if (TTF_Init() < 0) {
            error << "Failed to initialize SDL_ttf: " << TTF_GetError();
            throw(error.str());
        }

        

        if ((main_window = SDL_CreateWindow(
            app_title.c_str(),
            SDL_WINDOWPOS_CENTERED,			// Use Centered Top Coordinate
            SDL_WINDOWPOS_CENTERED,			// Use Centered Left Coordinate
            screen_width, screen_height,	// Width & Height
            0			// Flags (Window is implicitly shown if SDL_WINDOW_HIDDEN is not set)
        )) == 0) {
            error << "Failed to create a window: " << SDL_GetError();
            throw(error.str());
        }

        if ((renderer = SDL_CreateRenderer(main_window, -1, 0)) == 0) {
            error << "Failed to create the renderer: " << SDL_GetError();
            throw(error.str());
        }
        SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
        // CONTENT LOADING:
        // ------------------------------------------------------------------------------------------------------------

        SDL_Texture* grassland_tiles = NULL;
        {
            std::string grassland_tiles_surface_path = "content/grassland_tiles.png";
            SDL_Surface* grassland_tiles_surface = IMG_Load(grassland_tiles_surface_path.c_str());
            if (grassland_tiles_surface == NULL) {
                error << "Failed to load surface for '" << grassland_tiles_surface_path << "': " << IMG_GetError();
                throw(error.str());
            }

            grassland_tiles = SDL_CreateTextureFromSurface(renderer, grassland_tiles_surface);
            SDL_FreeSurface(grassland_tiles_surface);

            if (grassland_tiles == NULL) {
                error << "Failed to load texture for '" << grassland_tiles_surface_path << "': " << SDL_GetError();
                throw(error.str());
            }
        }

        TTF_Font* hud_font = TTF_OpenFont("content/Enigma__2.TTF", 32);
        if (hud_font == NULL) {
            error << "Failed to load font: " << TTF_GetError();
            throw(error.str());
        }

        // Map:

        auto main_camera = isometric::camera::create(0, 0, screen_width, screen_height, 0, 0);
        auto map = isometric::tile_map::create(1024, 1024, 64, 32);

        map->add_image(isometric::tile_image::create("selection", 0, grassland_tiles, 960, 160, tile_width, tile_height));
        map->set_selection_image(0);

        map->add_layer("grass");
        unsigned grass_source_x = 0;
        for (unsigned i = 1, grass_source_x = 0; i < 16; i++, grass_source_x += 64) {
            auto grass_tile_image = isometric::tile_image::create("grass" + std::to_string(i), i, grassland_tiles, grass_source_x, 0, tile_width, tile_height);
            map->add_image(grass_tile_image);
            map->add_layer_default_image("grass", i);
        }

        map->add_layer("foliage");
        auto bush1_tile_image = isometric::tile_image::create("bush1", 99, grassland_tiles, 512, 320, tile_width, tile_height * 2);
        map->add_image(bush1_tile_image);

        map->add_tile("foliage", 0, 0, isometric::tile(99));
        map->add_tile("foliage", 9, 9, isometric::tile(99));

        std::unique_ptr<isometric::world> world = std::make_unique<isometric::world>(map, main_camera);
        world->set_selection({ 9, 9 });

        // GAME LOOP START:
        // ------------------------------------------------------------------------------------------------------------

        // Setup the initial beforeTime before the game loop starts up...
        // SDL_GetPerformanceCounter is in a unit of measurement only meaningful to this computer
        // SDL_GetPerformanceFrequency is a value to divide by to convert the counter to seconds
        double beforeTime = (double)SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
        float speed = 200.0f; /* tiles per second of movement */

        // PRIMARY & EVENT LOOP:
        while (!should_quit) {
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT:
                    should_quit = 1;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        std::cout << "SDL_WINDOWEVENT_FOCUS_GAINED" << std::endl;
                        break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    std::cout << "Mouse Down" << std::endl;
                    break;
                case SDL_MOUSEBUTTONUP:
                    std::cout << "Mouse Up" << std::endl;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        should_quit = 1;
                        break;
                    }
                    break;
                }
            }

            // CALCULATE DELTA TIME:
            // --------------------------------------------------------------------------------------------------------
            // Get the current time by querying the performance counter and using the performance frequency to give it 
            // meaning (convert it to seconds)
            double currentTime = (double)SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
            float deltaTime = static_cast<float>(currentTime - beforeTime);
            beforeTime = currentTime; // Prime beforeTime for the next frame

            // MOVE MAP

            const Uint8* state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_LEFT]) {
                //main_camera->set_current_x(std::max(main_camera->get_current_x() - (speed * deltaTime), 0.0f));
                main_camera->set_current_x(main_camera->get_current_x() - (speed * deltaTime));
            }
            if (state[SDL_SCANCODE_RIGHT]) {
                //main_camera->set_current_x(std::min(main_camera->get_current_x() + (speed * deltaTime), static_cast<float>((int)map->get_map_width() - (int)world->get_max_horizontal_tiles() - 1)));
                main_camera->set_current_x(main_camera->get_current_x() + (speed * deltaTime));
            }
            if (state[SDL_SCANCODE_UP]) {
                //main_camera->set_current_y(std::max(main_camera->get_current_y() - ((speed * 2) * deltaTime), 0.0f));
                main_camera->set_current_y(main_camera->get_current_y() - ((speed * 2) * deltaTime));
            }
            if (state[SDL_SCANCODE_DOWN]) {
                //main_camera->set_current_y(std::min(main_camera->get_current_y() + ((speed * 2) * deltaTime), static_cast<float>((int)map->get_map_height() - (int)world->get_max_vertical_tiles() - 1)));
                main_camera->set_current_y(main_camera->get_current_y() + ((speed * 2) * deltaTime));
            }

            // RENDERING:
            // --------------------------------------------------------------------------------------------------------

            // Clear the background:
            SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255);
            SDL_RenderClear(renderer);

            world->update();
            world->render(renderer);

            // FPS
            time_since_last_fps += deltaTime;
            if (time_since_last_fps >= 0.25F || current_fps <= 0.0F) {
                time_since_last_fps = 0.0F;
                current_fps = 1.0 / deltaTime;
            }

            {
                int mouse_x = 0, mouse_y = 0;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                SDL_Point mouse_pos = { mouse_x, mouse_y };

                std::string fps_text = std::format("{: 4.1f} FPS", current_fps);
                std::string coord_text = std::format("Coordinate: {:04.0f}, {:04.0f}", main_camera->get_current_x(), main_camera->get_current_y());
                std::string other_text = std::format("Selection: {:04}, {:04}", world->get_selection().x, world->get_selection().y);

                SDL_Surface* fps_text_surface = TTF_RenderText_Blended(hud_font, fps_text.c_str(), { 255, 255, 255, 255 });
                SDL_Texture* fps_text_texture = SDL_CreateTextureFromSurface(renderer, fps_text_surface);

                SDL_Surface* coord_text_surface = TTF_RenderText_Blended(hud_font, coord_text.c_str(), { 255, 255, 255, 255 });
                SDL_Texture* coord_text_texture = SDL_CreateTextureFromSurface(renderer, coord_text_surface);

                SDL_Surface* other_text_surface = TTF_RenderText_Blended(hud_font, other_text.c_str(), { 255, 255, 255, 255 });
                SDL_Texture* other_text_texture = SDL_CreateTextureFromSurface(renderer, other_text_surface);

                int margin = 12;

                SDL_Rect fps_text_dstrect = { (screen_width - fps_text_surface->w) - margin, margin, fps_text_surface->w, fps_text_surface->h };
                SDL_RenderCopy(renderer, fps_text_texture, NULL, &fps_text_dstrect);

                SDL_Rect coord_text_dstrect = { (screen_width - coord_text_surface->w) - margin, (screen_height - coord_text_surface->h) - margin, coord_text_surface->w, coord_text_surface->h };
                SDL_RenderCopy(renderer, coord_text_texture, NULL, &coord_text_dstrect);

                SDL_Rect other_text_dstrect = { margin, (screen_height - other_text_surface->h) - margin, other_text_surface->w, other_text_surface->h };
                SDL_RenderCopy(renderer, other_text_texture, NULL, &other_text_dstrect);

                SDL_FreeSurface(fps_text_surface); SDL_DestroyTexture(fps_text_texture);
                SDL_FreeSurface(coord_text_surface); SDL_DestroyTexture(coord_text_texture);
                SDL_FreeSurface(other_text_surface); SDL_DestroyTexture(other_text_texture);
            }

            // Show on screen:
            SDL_RenderPresent(renderer);
        }
    }
    catch (std::string error_str) {
        // This is a simple way to show a message box, if main_window failed to create this will still work since 
        // main_window will be NULL (the message box will just not have a parent):
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, app_title.c_str(), error_str.c_str(), main_window);

        // Output the error to the console, if you have one
        std::cout << error_str << std::endl;
    }

    // CLEAN-UP & SHUTDOWN:
    // ----------------------------------------------------------------------------------------------------------------
    if (renderer) SDL_DestroyRenderer(renderer);
    if (main_window) SDL_DestroyWindow(main_window);
    SDL_Quit();

    return 0;
}