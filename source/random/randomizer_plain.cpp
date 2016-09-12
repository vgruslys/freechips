#include "randomizer_plain.h"
#include <random>
#include <algorithm>

void RandomizerPlain::seed()
{
    _mt.seed(_device());
}

RandomizerPlain::RandomizerPlain():_uniform(0,51)
{
    seed();
}

int RandomizerPlain::randInt(int a, int b)
{
    if(a == _uniform.min() && b == _uniform.max()) 
    {
        return _uniform(_mt);
    }
    else
    {
        _uniform = std::uniform_int_distribution<int>(a,b);
        return randInt(a,b);
    }
}

int RandomizerPlain::randWeights(int* array, int size)
{
    return 0;
}
