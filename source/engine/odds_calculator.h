#ifndef __ODDS_CALCULATOR__
#define __ODDS_CALCULATOR__
#include <utility>
#include "card.h"
class OddsCalculator
{
    public:
        virtual void setAccuracy(float) = 0; // The error in computing the probability 
        virtual void setConfidence(float) = 0; //Set the confidence probability that the error is within epsilon of real target
        virtual std::pair<float, float>  odds(Card, Card, Card, Card, Card*, int) = 0; //Compute hand odds, knowing the oppnent's cards
        virtual std::pair<float, float> odds(Card, Card, Card*, int) = 0; //Compute hand odds, not knowing the opponnent's cards
};
#endif