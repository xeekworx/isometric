#include "asset_management.h"
#include "../application/application.h"
#include <stdexcept>
#include <format>

using namespace isometric::assets;

asset_management::asset_management(SDL_Renderer* renderer)
    : renderer(renderer)
{
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Asset management constructed");
}

asset_management::~asset_management()
{
    shutdown();
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Asset management destructed");
}

const std::unique_ptr<asset>& asset_management::operator[](const std::string& name)
{
    if (asset_store.contains(name))
    {
        return asset_store.at(name);
    }
    else
    {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Asset not found in asset management: %s", name.c_str());
        throw std::runtime_error(std::format("Asset not found: '%s'", name));
    }
}

void asset_management::register_asset(std::unique_ptr<asset> new_asset)
{
    if (new_asset != nullptr)
    {
        asset_store[new_asset->get_name()] = std::move(new_asset);
    }
}

bool isometric::assets::asset_management::unregister_asset(const std::string& name)
{
    if (asset_store.contains(name))
    {
        auto& asset = (*this)[name];
        asset->clear();
        asset_store.erase(name);
        return true;
    }

    return false;
}

void asset_management::shutdown()
{
    asset_store.clear(); // The assets should all auto delete
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Asset management shutdown");
}
