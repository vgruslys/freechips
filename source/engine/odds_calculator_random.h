#ifndef __ODDS_CALCULATOR_RANDOM__
#define __ODDS_CALCULATOR_RANDOM__
#include <utility>
#include "card.h"
#include "random_deck_plain.h"
#include "odds_calculator.h"
#include "judge.h"

class OddsCalculatorRandom: public OddsCalculator
{
    public:
        void setAccuracy(float) final; // The error in computing the probability 
        void setConfidence(float) final ; //Set the confidence probability that the error is within epsilon of real target
        std::pair<float, float>  odds(const CardContainer &, const CardContainer&, const CardContainer&) final; //Compute hand odds
       
        OddsCalculatorRandom(Judge&);
    private:
        int _trials; //Number of trials
        float _confidence;
        float _accuracy;
        void updateTrials(); //Computes the number of trials required for a given confidence and accuracy
		
		RandomizerPlain _randomizer;
		Judge& _judge;
};
#endif