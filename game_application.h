#pragma once
#include "application.h"
#include "main_module.h"

namespace isometric::game {

    class game_application : public isometric::application
    {
    private:
        std::shared_ptr<main_module> main_module;

    protected:
        bool on_start() override;
    };

}