#pragma once
#include <SDL_ttf.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "asset.h"

namespace isometric::assets {

    class font : public asset
    {
    private:
        std::unordered_map<int, TTF_Font*> fonts;
        font(const std::string& name);

    public:
        static std::unique_ptr<font> load(const std::string& name, const std::string& path, int point_size);
        static std::unique_ptr<font> load(const std::string& name, const std::string& path, const std::vector<int>& point_sizes);

        TTF_Font* get_font(int point_size = 0) const;

        void clear() override;

        virtual ~font();
    };

}

