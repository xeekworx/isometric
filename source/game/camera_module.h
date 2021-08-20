#pragma once
#include <isometric.h>

namespace isometric::game {

    class camera_module : public isometric::module
    {
    private:
        std::shared_ptr<tile_map> map = nullptr;
        std::shared_ptr<world> world = nullptr;

    public:
        void setup(std::shared_ptr<tile_map> map, std::shared_ptr<isometric::world> world);

    protected:
        void on_registered() override;
        void on_unregister() override;

        void on_update(double delta_time) override;
        void on_late_update(double delta_time) override;
        void on_fixed_update(double fixed_delta_time) override;
    };

}