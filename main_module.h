#pragma once
#include "module.h"

namespace isometric::game {

    class main_module : public isometric::module
    {
    private:


    protected:
        void on_registered() override;
        void on_unregister() override;

        void on_update(double delta_time) override;
        void on_late_update(double delta_time) override;
        void on_fixed_update(double fixed_delta_time) override;
    };

}