#include "module.h"
#include "../source/application/application.h"

using namespace::isometric;

size_t module::module_count = 0;

module::module()
{
    if (!application::get_app() || !application::get_app()->is_initialized())
    {
        auto error_msg = std::string("Attempted to create module before an application object has been created and initialized");
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, error_msg.c_str());
        throw std::exception(error_msg.c_str());
    }

    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Module constructed");
}

module::~module()
{
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Module [%s] destructed", name.c_str());
}

bool module::set_enabled(bool enable)
{
    bool original_enabled_value = enabled;

    enabled = enable;

    return original_enabled_value;
}