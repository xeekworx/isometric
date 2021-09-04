#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "image.h"

namespace isometric::assets {

    class image_atlas : public image
    {
    private:
        std::unordered_map<std::string, size_t> subimage_names;
        std::vector<SDL_Rect> subimages;
        static constexpr SDL_Rect empty_rect{};

        image_atlas(const std::string& name, const std::string& path);

    public:
        static std::unique_ptr<image_atlas> load(const std::string& name, const std::string& path);

        void generate_subimages(unsigned width, unsigned height);
        size_t set_subimage(const SDL_Rect& srcrect, const std::string& name = "");
        void set_subimage_name(size_t index, const std::string& name);
        size_t get_subimage_index(const std::string& name, size_t default_index = 0) const;
        const SDL_Rect& get_subimage(const std::string& name) const;
        const SDL_Rect& get_subimage(size_t index) const;
        size_t subimage_count() const;

        void clear() override;
        virtual ~image_atlas();
    };

}

