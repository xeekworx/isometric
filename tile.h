#pragma once
#include <unordered_map>

namespace isometric {

    class tile
    {
    private:
        bool enabled = true;
        bool passable = true;
        bool empty = true;
        std::unordered_map<unsigned, unsigned> image_ids;

    public:
        tile(bool passable = true, bool enabled = true) : passable(passable), enabled(enabled),
            empty(false)
        { }

        bool is_empty() const
        {
            return empty;
        }

        bool is_passable() const
        {
            return passable;
        }

        bool is_enabled() const 
        {
            return enabled;
        }

        bool has_image(unsigned layer_id) const
        {
            return image_ids.contains(layer_id);
        }

        void set_image_id(unsigned layer_id, unsigned image_id)
        {
            image_ids[layer_id] = image_id;
            this->empty = false;
        }

        unsigned get_image_id(unsigned layer_id) const
        {
            if (image_ids.contains(layer_id))
            {
                return image_ids.at(layer_id);
            }
            else
            {
                return 0;
            }
        }

        const std::unordered_map<unsigned, unsigned>& get_image_ids() const
        {
            return image_ids;
        }
    };

}