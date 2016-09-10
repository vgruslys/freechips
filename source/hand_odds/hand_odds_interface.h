#ifndef __HAND_ODDS_INTERFACE__
#define __HAND_ODDS_INTERFACE__

#include "../global/error.h"
#include "../global/global_types.h"
#include "../global/card.h"
#include "../global/sets.h"

#include <utility>

class HandOdds {
    public:
        virtual pair<float,float> calculateOdds(int, Card*, Card*, Card*) const = 0;
}

#endif
