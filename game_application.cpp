#include "game_application.h"

using namespace isometric::game;

bool game_application::on_start()
{
    auto main_module = module::create<isometric::game::main_module>(true);
    register_module(main_module);

    return application::on_start();
}
