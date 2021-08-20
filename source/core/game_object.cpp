#include "game_object.h"
#include "world.h"

using namespace isometric;

game_object::game_object()
{

}

void game_object::setup_transform(std::shared_ptr<camera> camera, std::shared_ptr<tile_map> map)
{
    transform.reset(new isometric::transform(camera, map));
}
