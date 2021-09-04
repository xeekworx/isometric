#pragma once
#include <string>

namespace isometric::assets {

    class asset {
    private:
        std::string name;

    public:
        asset(const std::string& name) : name(name) {}
        virtual ~asset() {}

        const std::string& get_name() const
        {
            return name;
        }

        virtual void clear() = 0;
    };

}