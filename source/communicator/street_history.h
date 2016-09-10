#ifndef __STREET_HISTORY__
#define __STREET_HISTORY__

#include "error.h"
#include "global_types.h"
#include "logger.h"
#include "definable.h"
#include "action.h"

#include <algorithm>

class StreetHistory {

    public:

        StreetHistory(const Logger&);
        
        void launch();

        /* Methods for defining settings */

        void defineOwnTimebank(int);
        void defineOppTimebank(int);
        void defineOwnIncrement(int);
        void defineOppIncrement(int);
        void defineOwnStartingTime(int);
        void defineOppStartingTime(int);
        void defineOwnStartingStack(int);
        void defineOppStartingStack(int);
        void defineOwnStartingPot(int);
        void defineOppStartingPot(int);
        void defineStartingMinRaise(int);
        void defineStartingPlayer(Side);
        void makeThisPreflop(int, int);

        /* Methods for accessing settings */

        int getOwnTimebank() const;
        int getOppTimebank() const;
        int getOwnIncrement() const;
        int getOppIncrement() const;
        int getOwnStartingTime() const;
        int getOppStartingTime() const;
        int getOwnStartingStack() const;
        int getOppStartingStack() const;
        int getOwnStartingPot() const;
        int getOppStartingPot() const;
        int getStartingMinRaise() const;
        Side getStartingPlayer() const;
        int isPreflop() const;
        int getSmallBlind() const;
        int getBigBlind() const;

        /* Methods for accessing current parameters */

        int getPot() const;
        int getOwnPot() const;
        int getOppPot() const;
        int getOwnStack() const;
        int getOppStack() const;
        Side getPlayer() const;
        int getAmountToCall() const;
        int getOwnActed() const;
        int getOppActed() const;
        int getOwnPosted() const;
        int getOppPosted() const;
        int getOwnTime() const;
        int getOppTime() const;
        int getMinRaise() const;

        /* Methods for confirming properties of street */

        void confirmPot(int) const;
        void confirmAmountToCall(int) const;
        void confirmActionRequired(Side, int) const;
        void confirmOwnStack(int) const;
        void confirmOppStack(int) const;
        void confirmTime(Side, int) const;

        /* Methods for accessing winner of street */

        int isActive() const;
        int isLaunched() const;
        int isOwnFolded() const;
        int isOppFolded() const;
        int isOwnAllIn() const;
        int isOppAllIn() const;

        /* Methods for making actions */

        void makeAction(Side, ActionType, int, int);

    private:

        /* Methods for betting actions */

        void post(Side, int);
        void raise(Side, int);
        void call(Side);
        void check(Side);
        void fold(Side);
        void useTime(Side, int);
        void payOwn(int);
        void payOpp(int);
        void updateMinRaise(int);

        /* Methods for updating status of street */

        int isLastActionPassive() const;
        void checkIfStillActive();

        /* Variables to store settings */

        Definable<int> _own_timebank;
        Definable<int> _opp_timebank;
        Definable<int> _own_increment;
        Definable<int> _opp_increment;
        Definable<int> _own_starting_time;
        Definable<int> _opp_starting_time;
        Definable<int> _own_starting_stack;
        Definable<int> _opp_starting_stack;
        Definable<int> _own_starting_pot;
        Definable<int> _opp_starting_pot;
        Definable<Side> _starting_player;
        Definable<int> _starting_min_raise;
        
        int _is_preflop;
        int _small_blind;
        int _big_blind;

        /* Variables to store status */

        int _is_active;
        int _is_launched;
        int _own_folded;
        int _opp_folded;
        int _own_acted;
        int _opp_acted;
        int _own_posted;
        int _opp_posted;
        int _is_last_action_passive;

        /* Variables to store current parameters */

        int _own_stack;
        int _opp_stack;
        int _own_pot;
        int _opp_pot;
        int _own_time;
        int _opp_time;
        Side _player;
        int _min_raise;

        /* Internal variables */

        const Logger& _logger;

};

#endif
