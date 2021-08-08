#pragma once
#include <random>

namespace isometric {

    class random
    {
    private:
        static std::random_device rd;
        static std::mt19937 gen;

        random() { } // Force as a static class

    public:
        static int range(int min_inclusive, int max_inclusive);
        static unsigned range(unsigned min_inclusive, unsigned max_inclusive);
    };

}