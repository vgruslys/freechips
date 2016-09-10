#ifndef __HISTORY__
#define __HISTORY__

#include "definable.h"
#include "error.h"
#include "global_types.h"
#include "logger.h"
#include "action.h"
#include "hand_history.h"
#include "street_history.h"
#include "blinds_tracker.h"

#include <vector>

class History {

    public:
    
        History(const Logger&);

        /* Methods for defining settings */

        void defineStartingSmallBlind(int);
        void defineStartingBigBlind(int);
        void defineHandsUntilChange(int);
        void defineOwnTimebank(int);
        void defineOppTimebank(int);
        void defineOwnIncrement(int);
        void defineOppIncrement(int);
        void defineOwnStartingStack(int);
        void defineOppStartingStack(int);
        void defineStartingPlayer(Side);

        /* Methods for accessing settings */
        
        int getStartingSmallBlind() const;
        int getStartingBigBlind() const;
        int getHandsUntilChange() const;
        int getOwnTimebank() const;
        int getOppTimebank() const;
        int getOwnIncrement() const;
        int getOppIncrement() const;
        int getOwnStartingStack() const;
        int getOppStartingStack() const;
        Side getStartingPlayer() const;

        /* Methods for confirming properties of hand */
        /* They call identical methods in hand */

        void confirmSmallBlind(int) const;
        void confirmBigBlind(int) const;
        void confirmStartingPlayer(Side) const;
        void confirmWinner(Side, int) const;

        /* Methods for confirming properties of street */
        /* They call identical methods of hand */

        void confirmPot(int) const;
        void confirmAmountToCall(int) const;
        void confirmActionRequired(Side, int) const;
        void confirmOwnStack(int) const;
        void confirmOppStack(int) const;

        /* Methods for updating hand */

        void startNewHand(int);
        void setCommunityCards(int, Card*);
        void setOwnCards(Card, Card);
        void setOppCards(Card, Card);

        /* Methods for updating street */
        /* They call identical methods of street */

        void startNewStreet();
        void makeAction(Side, ActionType, int, int);

        /* Methods for accessing hands and streets */

        int totalHands() const;
        const HandHistory& getHand(int) const;
        const HandHistory& getLastHand() const;
        const StreetHistory& getLastStreet() const;
        HandHistory& getHandToModify(int);
        HandHistory& getLastHandToModify();
        StreetHistory& getLastStreetToModify();

    private:

        /* Methods for controlling the status of history */

        void updateBlinds();
        void initialiseBlindsTracker();

        /* Methods for tracking if the commands come in the right order */

        int isReadyToStartNewHand() const;
        
        /* Methods for creating new hand */

        Side computeNewPlayer() const;
        void computeNewBlinds(int&, int&);
        int computeNewOwnTime() const;
        int computeNewOppTime() const;
        int computeNewOwnStack() const;
        int computeNewOppStack() const;

        /* Methods for accessing internal variables */

        const Logger& getLogger() const;
        BlindsTracker& getBlindsTracker();

        /* Variables to store settings */

        Definable<int> _starting_small_blind;
        Definable<int> _starting_big_blind;
        Definable<int> _hands_until_change;
        BlindsTracker _blinds_tracker;
        Definable<int> _own_timebank;
        Definable<int> _opp_timebank;
        Definable<int> _own_increment;
        Definable<int> _opp_increment;
        Definable<int> _own_starting_stack;
        Definable<int> _opp_starting_stack;
        Definable<Side> _starting_player;

        /* Variables for storing hands */

        std::vector<HandHistory> _hands;

        /* Internal variables */

        const Logger& _logger;

};

#endif
