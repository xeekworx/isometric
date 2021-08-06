#include <memory>
#include "application.h"

int main(int argc, char* argv[])
{
    {
        isometric::application_setup setup;
        setup.name = "Isometric Lab";
        setup.background_color = 0x006bFFFF;
        setup.shutdown_on_esc = true;

        auto app = std::make_unique<isometric::application>(setup);
        app->start();
    }

    return 0;
}