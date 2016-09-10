#ifndef __HAND_HISTORY__
#define __HAND_HISTORY__

#include "error.h"
#include "global_types.h"
#include "logger.h"
#include "definable.h"
#include "action.h"
#include "card.h"
#include "sets.h"
#include "street_history.h"


class HandHistory {
    
    public:

        HandHistory(const Logger&);

        void launch();
        
        /* Methods for defining settings */

        void defineSmallBlind(int);
        void defineBigBlind(int);
        void defineOwnTimebank(int);
        void defineOppTimebank(int);
        void defineOwnIncrement(int);
        void defineOppIncrement(int);
        void defineOwnStartingTime(int);
        void defineOppStartingTime(int);
        void defineOwnStartingStack(int);
        void defineOppStartingStack(int);
        void defineStartingPlayer(Side);

        /* Methods for accessing settings */

        int getSmallBlind() const;
        int getBigBlind() const;
        int getOwnTimebank() const;
        int getOppTimebank() const;
        int getOwnIncrement() const;
        int getOppIncrement() const;
        int getOwnStartingTime() const;
        int getOppStartingTime() const;
        int getOwnStartingStack() const;
        int getOppStartingStack() const;
        Side getStartingPlayer() const;

        /* Methods for accessing current parameters */

        int getOwnTime() const;
        int getOppTime() const;
        int getOwnStack() const;
        int getOppStack() const;
        int getOwnPot() const;
        int getOppPot() const;
        int getMinRaise() const;
        Side getPlayer() const;

        /* Methods for accessing outcome of hand */

        int isActive() const;
        Side getWinner() const;
        int getOwnWinAmount() const;
        int getOppWinAmount() const;

        /* Methods for confirming properties of hand */

        void confirmSmallBlind(int) const;
        void confirmBigBlind(int) const;
        void confirmStartingPlayer(Side) const;
        void confirmWinner(Side, int) const;

        /* Methods for confirming properties of street */
        /* They call identical methods of street */

        void confirmPot(int) const;
        void confirmAmountToCall(int) const;
        void confirmActionRequired(Side, int) const;
        void confirmOwnStack(int) const;
        void confirmOppStack(int) const;

        /* Methods for updating cards */

        void setCommunityCards(int, Card*);
        void setOwnCards(Card, Card);
        void setOppCards(Card, Card);

        /* Methods for accessing cards */

        int totalCommunityCards() const;
        int isOwnCardsVisible() const;
        int isOppCardsVisible() const;
        Card* getCommunityCards();
        void getOwnCards(Card&, Card&) const;
        void getOppCards(Card&, Card&) const;

        /* Methods for updating street */

        void startNewStreet();
        void makeAction(Side, ActionType, int, int);

        /* Methods for accessing streets */

        Street lastStreet() const;
        const StreetHistory& getPreflop() const;
        const StreetHistory& getFlop() const;
        const StreetHistory& getTurn() const;
        const StreetHistory& getRiver() const;
        const StreetHistory& getStreet(Street) const;
        const StreetHistory& getLastStreet() const;
        StreetHistory& getPreflopToModify();
        StreetHistory& getFlopToModify();
        StreetHistory& getTurnToModify();
        StreetHistory& getRiverToModify();
        StreetHistory& getStreetToModify(Street);
        StreetHistory& getLastStreetToModify();

    private:

        /* Methods for tracking if commands come in the right order */

        int isReadyToCreateStreet() const;
        int isShowdown() const;

        /* Internal methods for updating hand */

        void addCommunityCard(Card);
        void advanceStreetCounter();
        void checkForWinner();
        void declareWinner(Side, int, int);

        /* Methods for accessing internal variables */

        const Logger& getLogger() const;

        /* Variables to store settings */

        Definable<int> _small_blind;
        Definable<int> _big_blind;
        Definable<int> _own_timebank;
        Definable<int> _opp_timebank;
        Definable<int> _own_increment;
        Definable<int> _opp_increment;
        Definable<int> _own_starting_time;
        Definable<int> _opp_starting_time;
        Definable<int> _own_starting_stack;
        Definable<int> _opp_starting_stack;
        Definable<Side> _starting_player;

        /* Variables to store outcome of hand */

        Definable<Side> _winner;
        int _own_win_amount;
        int _opp_win_amount;

        /* Variables to store cards */
        
        Card _community_cards[5];
        Card _own_cards[2];
        Card _opp_cards[2];
        int _total_community_cards;
        int _total_own_cards;
        int _total_opp_cards;

        /* Variables to store streets */

        Street _last_street;
        StreetHistory _preflop;
        StreetHistory _flop;
        StreetHistory _turn;
        StreetHistory _river;

        /* Internal variables */

        const Logger& _logger;
};

#endif
