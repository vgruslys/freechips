#ifndef __ODDS_CALCULATOR__
#define __ODDS_CALCULATOR__
#include <utility>
#include "card.h"
#include "card_container.h"

class OddsCalculator
{
    public:
        virtual void setAccuracy(float) = 0; // The error in computing the probability 
        virtual void setConfidence(float) = 0; //Set the confidence probability that the error is within epsilon of real target
        virtual std::pair<float, float>  odds(const CardContainer &, const CardContainer&, const CardContainer&) = 0; //Compute hand odds
};
#endif