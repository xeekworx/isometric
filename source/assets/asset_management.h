#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <memory>
#include <unordered_map>
#include "asset.h"
#include "image.h"
#include "font.h"

namespace isometric {
    class application;
}

namespace isometric::assets {

    class asset_management
    {
        friend class application;
    private:
        SDL_Renderer* renderer = nullptr;
        std::unordered_map<std::string, std::unique_ptr<asset>> asset_store;

        asset_management(SDL_Renderer* renderer);

    public:

        const std::unique_ptr<asset>& operator[](const std::string& name);

        void register_asset(std::unique_ptr<asset> new_asset);
        bool unregister_asset(const std::string& name);

        void shutdown();
        ~asset_management();
    };

}