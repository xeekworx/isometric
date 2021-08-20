#pragma once
#include <SDL.h>
#include <string>
#include <memory>
#include <type_traits>

namespace isometric {

    class application;

    class module
    {
        friend application;
    private:
        std::shared_ptr<application> app = nullptr;
        static size_t module_count;
        std::string name;
        bool enabled = true;

    public:
        virtual ~module();

        template<class T> static std::shared_ptr<T> create(const std::string& name, bool enabled = true);
        template<class T> static std::shared_ptr<T> create(bool enabled = true);

        bool is_enabled() const { return enabled; }
        bool set_enabled(bool enable = true);

    protected:
        module();

        virtual void on_registered() {}
        virtual void on_unregister() {}

        virtual void on_update(double delta_time) {}
        virtual void on_late_update(double delta_time) {}
        virtual void on_fixed_update(double fixed_delta_time) {}
    };

    template<class T>
    static std::shared_ptr<T> module::create(const std::string& name, bool enabled)
    {
        if (std::is_convertible<T, module>())
        {
            std::shared_ptr<T> new_module = std::shared_ptr<T>(new T);
            module_count++;
            new_module->enabled = true;
            new_module->name = name.empty() ? "Unnamed Module " + std::to_string(module_count) : name;

            SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Module [%s] created", new_module->name.c_str());

            return new_module;
        }
        else
        {
            throw std::exception("Failed to create module because the given type was not derived from module");
        }
    }

    template<class T>
    static std::shared_ptr<T> module::create(bool enabled)
    {
        return create<T>(std::string(), enabled);
    }
}