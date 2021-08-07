#include <memory>
#include "application.h"
#include "module.h"

using namespace isometric;

class mymodule : public isometric::module {

};

class myapplication : public isometric::application {
protected:
    bool on_start() override {

        auto main_module = module::create<mymodule>(true);
        register_module(main_module);

        return application::on_start();
    }
};

int main(int argc, char* argv[])
{
    try {
        application_setup setup;
        setup.name = "Isometric Lab";
        setup.background_color = 0x006bFFFF;
        setup.shutdown_on_esc = true;
        setup.verbose_logging = true;
        setup.vertical_sync = true;
        setup.broadcast_fps = true;

        auto app = application::create<myapplication>(setup);

        app->start();
    }
    catch (std::exception ex) {
        return -1;
    }

    return 0;
}