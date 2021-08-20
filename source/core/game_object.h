#pragma once
#include <SDL.h>
#include <memory>
#include "transform.h"

namespace isometric {

    class game_object
    {
    protected:
        std::unique_ptr<transform> transform = nullptr;

    public:
        game_object();
        virtual ~game_object() {}

        /// <summary>
        /// Generally, only used by the world class to configure the transform
        /// </summary>
        /// <param name="camera"></param>
        /// <param name="map"></param>
        void setup_transform(std::shared_ptr<camera> camera, std::shared_ptr<tile_map> map);

        virtual void on_render(SDL_Renderer* renderer, double delta_time) = 0;
    };

}