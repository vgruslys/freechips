#ifndef __ODDS_CALCULATOR_RANDOM__
#define __ODDS_CALCULATOR_RANDOM__
#include <utility>
#include "card.h"
#include "odds_calculator.h"
#include "judge.h"
#include "ranvec1.h"

class OddsCalculatorRandom: public OddsCalculator
{
    public:
        std::pair<float, float>  odds(const CardContainer &, const CardContainer&, const CardContainer&) final; //Compute hand odds
       
        OddsCalculatorRandom(Judge&);
    private:
        int _trials; //Number of trials
		Judge& _judge;
		Ranvec1 _rgen;
};
#endif