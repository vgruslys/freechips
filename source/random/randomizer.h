#ifndef __RANDOMIZER__
#define __RANDOMIZER__

class Randomizer
{
    public:
        virtual int randInt(int, int) = 0; // draw an int in range [a,b]
        virtual int randWeights(int*, int) = 0; //returns an index based on weights found in array
};
#endif
