#ifndef __ODDS_CALCULATOR__
#define __ODDS_CALCULATOR__
#include <utility>
#include "card.h"
#include "card_container.h"

class OddsCalculator
{
    public:
        virtual std::pair<float, float>  odds(const CardContainer&,const CardContainer&, const CardContainer&) = 0; //Compute hand odds
};
#endif