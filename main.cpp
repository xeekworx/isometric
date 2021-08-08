#include <memory>
#include "game_application.h"

using namespace isometric;
using namespace isometric::game;

int main(int argc, char* argv[])
{
    try {
        application_setup setup;
        setup.name = "Isometric Lab";
        setup.background_color = 0x006bFFFF;
        setup.shutdown_on_esc = true;
        setup.verbose_logging = true;
        setup.vertical_sync = false;
        setup.broadcast_fps = true;

        auto app = application::create<game_application>(setup);

        app->start();
    }
    catch (std::exception ex) {
        return -1;
    }

    return 0;
}