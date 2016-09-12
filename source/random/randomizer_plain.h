#ifndef __RANDOMIZER_PLAIN__
#define __RANDOMIZER_PLAIN__
#include <random>
#include "randomizer.h"
class RandomizerPlain: public Randomizer
{
public:
    int randInt(int, int); //Returns a (uniform) random int in range [a,b]
    int randWeights(int*, int); 
    RandomizerPlain();
    
private:
    void seed(); //To seed the pseudo random generator 
    std::random_device _device; //To generate a random seed
    std::mt19937_64 _mt; //Pseudo random generator
    std::uniform_int_distribution<int> _uniform; //Uniform distribution
};
#endif