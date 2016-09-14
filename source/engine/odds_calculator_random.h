#ifndef __ODDS_CALCULATOR_RANDOM__
#define __ODDS_CALCULATOR_RANDOM__
#include <utility>
#include "card.h"
#include "random_deck_plain.h"
#include "odds_calculator.h"
class OddsCalculatorRandom: public OddsCalculator
{
    public:
        void setAccuracy(float); // The error in computing the probability 
        void setConfidence(float) ; //Set the confidence probability that the error is within epsilon of real target
        std::pair<float, float>  handOdds(Card, Card, Card, Card, Card*, int); //Compute hand odds, knowing the oppnent's cards
        std::pair<float, float> handOdds(Card, Card, Card*, int); //Compute hand odds, not knowing the opponnent's cards
        
        OddsCalculatorRandom();
    private:
        int _trials; //Number of trials
        float _confidence;
        float _accuracy;
        void updateTrials(); //Computes the number of trials required for a given confidence and accuracy
        RandomDeckPlain _rdeck;
};
#endif