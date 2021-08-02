#pragma once

namespace isometric {

    class tile
    {
    private:
        bool enabled = true;
        bool passable = true;
        unsigned image_id = 0;
        bool empty = true;

    public:
        tile() { }

        tile(unsigned image_id, bool passable = true, bool enabled = true) :
            image_id(image_id), passable(passable), enabled(enabled),
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

        void set_image_id(unsigned image_id)
        {
            this->image_id = image_id;
            this->empty = false;
        }

        unsigned get_image_id() const
        {
            return image_id;
        }
    };

}