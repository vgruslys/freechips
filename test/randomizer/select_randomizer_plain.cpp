#include "test.h"
#include "randomizer_plain.h"

Randomizer* selectRandomizer()
{
    return new RandomizerPlain();
}
