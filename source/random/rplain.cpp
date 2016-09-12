#include "rplain.h"
#include <random>
#include <algorithm>

void RPlain::seed()
{
    _mt.seed(_device());
}

RPlain::RPlain():_uniform(0,51)
{
    seed();
}

int RPlain::randInt(int a, int b)
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

int RPlain::randWeights(int* array, int size)
{
    return 0;
}
