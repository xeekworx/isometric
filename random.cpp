#include "random.h"
#include <time.h>

using namespace isometric;

std::random_device random::rd;
std::mt19937 random::gen(rd());

int random::range(int min_inclusive, int max_inclusive)
{
    std::uniform_int_distribution<int> dis(min_inclusive, max_inclusive);
    return dis(gen);
}

unsigned random::range(unsigned min_inclusive, unsigned max_inclusive)
{
    std::uniform_int_distribution<unsigned> dis(min_inclusive, max_inclusive);
    return dis(gen);
}