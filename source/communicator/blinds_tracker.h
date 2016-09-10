#ifndef __BLINDS_TRACKER__
#define __BLINDS_TRACKER__

#include "error.h"

#include <vector>

class BlindsTracker {

    public:

        BlindsTracker();
        ~BlindsTracker();

        void initialise(int, int, int);
        int isInitialised() const;
        
        int getSmallBlind() const;
        int getBigBlind() const;
        void advance();
        
    private:
        
        int* _small_blind;
        int* _big_blind;
        int _final_small_blind;
        int _final_big_blind;

        int _counter;
        int _total_levels;
        int _hands_until_change;
        int _initialised;
};

#endif
