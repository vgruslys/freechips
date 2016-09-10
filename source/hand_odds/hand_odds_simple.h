#ifndef __HAND_ODDS_SIMPLE__
#define __HAND_ODDS_SIMPLE__

#include "hand_odds_interface.h"

class HandOddsSimple : public HandOdds {
    public:
        virtual pair<float,float> calculateOdds(int, Card*, Card*, Card*) const;

    private:
        pair<float,float> calculateOddsNoTable(Card*, Card*) const;
        int8_t cardToInt(Card) const;
}
#endif
