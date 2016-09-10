#include "hand_history.h"

HandHistory :: HandHistory(const Logger& logger)
    : _logger(logger),
      _preflop(logger),
      _flop(logger),
      _turn(logger),
      _river(logger)
{
    _last_street = NULL_STREET;
    _total_community_cards = 0;
    _total_own_cards = 0;
    _total_opp_cards = 0;
}

void HandHistory :: launch()
{
    if (lastStreet() != NULL_STREET)
        throw HistoryError("Hands was already lauched");

    startNewStreet();
}

/* Methods for defining settings */

void HandHistory :: defineSmallBlind(int blind)
{
    _small_blind.set(blind);
}

void HandHistory :: defineBigBlind(int blind)
{
    _big_blind.set(blind);
}

void HandHistory :: defineOwnTimebank(int timebank)
{
    _own_timebank.set(timebank);
}

void HandHistory :: defineOppTimebank(int timebank)
{
    _opp_timebank.set(timebank);
}

void HandHistory :: defineOwnIncrement(int increment)
{
    _own_increment.set(increment);
}

void HandHistory :: defineOppIncrement(int increment)
{
    _opp_increment.set(increment);
}

void HandHistory :: defineOwnStartingTime(int time)
{
    _own_starting_time.set(time);
}

void HandHistory :: defineOppStartingTime(int time)
{
    _opp_starting_time.set(time);
}

void HandHistory :: defineOwnStartingStack(int stack)
{
    _own_starting_stack.set(stack);
}

void HandHistory :: defineOppStartingStack(int stack)
{
    _opp_starting_stack.set(stack);
}

void HandHistory :: defineStartingPlayer(Side player)
{
    _starting_player.set(player);
}

/* Methods for accessing settings */

int HandHistory :: getSmallBlind() const
{
    return _small_blind.get();
}

int HandHistory :: getBigBlind() const
{
    return _big_blind.get();
}

int HandHistory :: getOwnTimebank() const
{
    return _own_timebank.get();
}

int HandHistory :: getOppTimebank() const
{
    return _opp_timebank.get();
}

int HandHistory :: getOwnIncrement() const
{
    return _own_increment.get();
}

int HandHistory :: getOppIncrement() const
{
    return _opp_increment.get();
}

int HandHistory :: getOwnStartingTime() const
{
    return _own_starting_time.get();
}

int HandHistory :: getOppStartingTime() const
{
    return _opp_starting_time.get();
}

int HandHistory :: getOwnStartingStack() const
{
    return _own_starting_stack.get();
}

int HandHistory :: getOppStartingStack() const
{
    return _opp_starting_stack.get();
}

Side HandHistory :: getStartingPlayer() const
{
    return _starting_player.get();
}

/* Methods for accessing current parameters */

int HandHistory :: getOwnTime() const
{
    if (lastStreet() == NULL_STREET)
        return getOwnStartingTime();
    else
        return getLastStreet().getOwnTime();
}

int HandHistory :: getOppTime() const
{
    if (lastStreet() == NULL_STREET)
        return getOppStartingTime();
    else
        return getLastStreet().getOppTime();
}

int HandHistory :: getOwnStack() const
{
    if (lastStreet() == NULL_STREET)
        return getOwnStartingStack();
    else if (isActive())
        return getLastStreet().getOwnStack();
    else
        return getOwnStartingStack() + getOwnWinAmount();
}

int HandHistory :: getOppStack() const
{
    if (lastStreet() == NULL_STREET)
        return getOppStartingStack();
    else if (isActive())
        return getLastStreet().getOppStack();
    else
        return getOppStartingStack() + getOppWinAmount();
}

int HandHistory :: getOwnPot() const
{
    if (lastStreet() == NULL_STREET)
        return 0;
    else if (isActive())
        return getLastStreet().getOwnPot();
    else
        return 0;
}

int HandHistory :: getOppPot() const
{
    if (lastStreet() == NULL_STREET)
        return 0;
    else if (isActive())
        return getLastStreet().getOppPot();
    else
        return 0;
}

int HandHistory :: getMinRaise() const
{
    if (lastStreet() == NULL_STREET)
        return getBigBlind();
    else
        return getLastStreet().getOwnPot();
}

Side HandHistory :: getPlayer() const
{
    if (lastStreet() == NULL_STREET)
        return getStartingPlayer();
    else if (getLastStreet().isActive())
        return getLastStreet().getPlayer();
    else
        return getStartingPlayer();
}

/* Methods for accessing outcome of hand */

int HandHistory :: isActive() const
{
    return !_winner.isDefined();
}

Side HandHistory :: getWinner() const
{
    if (isActive())
        throw HistoryError("There is no winner: hand is active");
    else
        return _winner.get();
}

int HandHistory :: getOwnWinAmount() const
{
    if (isActive())
        throw HistoryError("There are no winnings: hand is active");
    else
        return _own_win_amount;
}

int HandHistory :: getOppWinAmount() const
{
    if (isActive())
        throw HistoryError("There are no winnings: hand is active");
    else
        return _opp_win_amount;
}

/* Methods for confirming properties of hand */

void HandHistory :: confirmSmallBlind(int blind) const
{
    if (blind != getSmallBlind())
        throw HistoryError("Inconsistent small blind");
}

void HandHistory :: confirmBigBlind(int blind) const
{
    getLogger().log(L_DEBUG, "actual_bb = %d; bb = %d;\n", getBigBlind(), blind);
    if (blind != getBigBlind())
        throw HistoryError("Inconsistent big blind");
}

void HandHistory :: confirmStartingPlayer(Side player) const
{
    if (player != getStartingPlayer())
        throw HistoryError("Inconsistent starting player");
}

void HandHistory :: confirmWinner(Side player, int amount) const
{
    Side winner = getWinner();
    int own_pot = getLastStreet().getOwnPot();
    int opp_pot = getLastStreet().getOppPot();
    int min_pot = std::min(own_pot, opp_pot);
    int own_winnings;
    int opp_winnings;

    if (winner == ME) {
        own_winnings = 2 * min_pot;
        opp_winnings = opp_pot - min_pot;
    }
    else if (winner == OPP) {
        opp_winnings = 2 * min_pot;
        own_winnings = own_pot - min_pot;
    }
    else {
        own_winnings = own_pot;
        opp_winnings = opp_pot;
    }

    if (player == ME) {
        if (own_winnings == 0)
            throw HistoryError("Inconsistent winner");
        else if (own_winnings != amount)
            throw HistoryError("Inconsistent winning amount");
    }
    else {
        if (opp_winnings == 0)
            throw HistoryError("Inconsistent winner");
        else if (opp_winnings != amount)
            throw HistoryError("Inconsistent winning amount");
    }
}

/* Methods for confirming properties of street */

void HandHistory :: confirmPot(int pot) const
{
    getLastStreet().confirmPot(pot);
}

void HandHistory :: confirmAmountToCall(int amount) const
{
    getLastStreet().confirmAmountToCall(amount);
}

void HandHistory :: confirmActionRequired(Side player, int time) const
{
    getLastStreet().confirmActionRequired(player, time);
}

void HandHistory :: confirmOwnStack(int stack) const
{
    getLastStreet().confirmOwnStack(stack);
}

void HandHistory :: confirmOppStack(int stack) const
{
    getLastStreet().confirmOppStack(stack);
}

/* Methods for updating cards */

void HandHistory :: setCommunityCards(int total_cards, Card* cards)
{
    int curr_total = totalCommunityCards();

    if (curr_total > total_cards)
        throw HistoryError("More community cards are visible");
    else {
        for (int i = 0; i < curr_total; ++i)
            if (getCommunityCards()[i] != cards[i])
                throw HistoryError("Community cards have changed");
        for (int i = curr_total; i < total_cards; ++i)
            addCommunityCard(cards[i]);
    }
    checkForWinner();
}

void HandHistory :: setOwnCards(Card card1, Card card2)
{
    if (isOwnCardsVisible()) {
        Card my_card1;
        Card my_card2;
        getOwnCards(my_card1, my_card2);
        if ((card1 != my_card1) || (card2 != my_card2))
            throw HistoryError("My cards have changed");
    } else {
        _total_own_cards = 2;
        _own_cards[0] = card1;
        _own_cards[1] = card2;
    }
    checkForWinner();
}

void HandHistory :: setOppCards(Card card1, Card card2)
{
    if (isOppCardsVisible()) {
        Card opp_card1;
        Card opp_card2;
        getOwnCards(opp_card1, opp_card2);
        if ((card1 != opp_card1) || (card2 != opp_card2))
            throw HistoryError("My cards have changed");
    } else {
        _total_opp_cards = 2;
        _opp_cards[0] = card1;
        _opp_cards[1] = card2;
    }
    checkForWinner();
}

/* Methods for accessing cards */

int HandHistory :: totalCommunityCards() const
{
    return _total_community_cards;
}

int HandHistory :: isOwnCardsVisible() const
{
    if (_total_own_cards == 0)
        return 0;
    else if (_total_own_cards == 2)
        return 1;
    else
        throw HistoryError("Unexpected error related to own cards");
}

int HandHistory :: isOppCardsVisible() const
{
    if (_total_opp_cards == 0)
        return 0;
    else if (_total_opp_cards == 2)
        return 1;
    else
        throw HistoryError("Unexpected error related to opponent cards");
}

Card* HandHistory :: getCommunityCards()
{
    return _community_cards;
}

void HandHistory :: getOwnCards(Card& card1, Card& card2) const
{
    if (!isOwnCardsVisible())
        throw HistoryError("My cards are not visible");
    else {
        card1 = _own_cards[0];
        card2 = _own_cards[1];
    }
}

void HandHistory :: getOppCards(Card& card1, Card& card2) const
{
    if (!isOppCardsVisible())
        throw HistoryError("Opponent cards are not visible");
    else {
        card1 = _opp_cards[0];
        card2 = _opp_cards[1];
    }
}

/* Methods for updating street */

void HandHistory :: startNewStreet()
{
    if (!isReadyToCreateStreet())
        throw HistoryError("Not ready to create street");

    int own_timebank = getOwnTimebank();
    int opp_timebank = getOppTimebank();
    int own_increment = getOwnIncrement();
    int opp_increment = getOppIncrement();
    int own_time = getOwnTime();
    int opp_time = getOppTime();
    int own_stack = getOwnStack();
    int opp_stack = getOppStack();
    int own_pot = getOwnPot();
    int opp_pot = getOppPot();
    int min_raise = getMinRaise();
    Side player = getPlayer();

    advanceStreetCounter();

    getLastStreetToModify().defineOwnTimebank(own_timebank);
    getLastStreetToModify().defineOppTimebank(opp_timebank);
    getLastStreetToModify().defineOwnIncrement(own_increment);
    getLastStreetToModify().defineOppIncrement(opp_increment);
    getLastStreetToModify().defineOwnStartingTime(own_time);
    getLastStreetToModify().defineOppStartingTime(opp_time);
    getLastStreetToModify().defineOwnStartingStack(own_stack);
    getLastStreetToModify().defineOppStartingStack(opp_stack);
    getLastStreetToModify().defineOwnStartingPot(own_pot);
    getLastStreetToModify().defineOppStartingPot(opp_pot);
    getLastStreetToModify().defineStartingMinRaise(min_raise);
    getLastStreetToModify().defineStartingPlayer(player);

    if (lastStreet() == PREFLOP) {
        int small_blind = getSmallBlind();
        int big_blind = getBigBlind();
        getLastStreetToModify().makeThisPreflop(small_blind, big_blind);
    }

    getLastStreetToModify().launch();
}

void HandHistory :: makeAction(Side player, ActionType type, int value, int time)
{
    getLastStreetToModify().makeAction(player, type, value, time);
    checkForWinner();
}

/* Methods for accessing streets */

Street HandHistory :: lastStreet() const
{
    return _last_street;
}

const StreetHistory& HandHistory :: getPreflop() const
{
    if (streetCompare(PREFLOP, lastStreet()) != -1)
        return _preflop;
    else
        throw HistoryError("There is no preflop yet");
}

const StreetHistory& HandHistory :: getFlop() const
{
    if (streetCompare(FLOP, lastStreet()) != -1)
        return _flop;
    else
        throw HistoryError("There is no flop yet");
}

const StreetHistory& HandHistory :: getTurn() const
{
    if (streetCompare(TURN, lastStreet()) != -1)
        return _turn;
    else
        throw HistoryError("There is no turn yet");
}

const StreetHistory& HandHistory :: getRiver() const
{
    if (streetCompare(RIVER, lastStreet()) != -1)
        return _river;
    else
        throw HistoryError("There is no river yet");
}

const StreetHistory& HandHistory :: getStreet(Street street) const
{
    switch (street) {

        case PREFLOP :
            return getPreflop();
            break;

        case FLOP :
            return getFlop();
            break;

        case TURN :
            return getTurn();
            break;

        case RIVER :
            return getRiver();
            break;

        default :
            throw HistoryError("Trying to access invalid street");
    }
}

const StreetHistory& HandHistory :: getLastStreet() const
{
    if (lastStreet() == NULL_STREET)
        throw HistoryError("There are no streets yet");
    else
        return getStreet(lastStreet());
}

StreetHistory& HandHistory :: getPreflopToModify()
{
    if (streetCompare(PREFLOP, lastStreet()) != -1)
        return _preflop;
    else
        throw HistoryError("There is no preflop yet");
}

StreetHistory& HandHistory :: getFlopToModify()
{
    if (streetCompare(FLOP, lastStreet()) != -1)
        return _flop;
    else
        throw HistoryError("There is no flop yet");
}

StreetHistory& HandHistory :: getTurnToModify()
{
    if (streetCompare(TURN, lastStreet()) != -1)
        return _turn;
    else
        throw HistoryError("There is no turn yet");
}

StreetHistory& HandHistory :: getRiverToModify()
{
    if (streetCompare(RIVER, lastStreet()) != -1)
        return _river;
    else
        throw HistoryError("There is no river yet");
}

StreetHistory& HandHistory :: getStreetToModify(Street street)
{
    switch (street) {

        case PREFLOP :
            return getPreflopToModify();
            break;

        case FLOP :
            return getFlopToModify();
            break;

        case TURN :
            return getTurnToModify();
            break;

        case RIVER :
            return getRiverToModify();
            break;

        default :
            throw HistoryError("Trying to access invalid street");
    }
}

StreetHistory& HandHistory :: getLastStreetToModify()
{
    if (lastStreet() == NULL_STREET)
        throw HistoryError("There are no streets yet");
    else
        return getStreetToModify(lastStreet());
}

/* Methods for tracking if commands come in the right order */

int HandHistory :: isReadyToCreateStreet() const
{
    if (lastStreet() == NULL_STREET)
        return
            _small_blind.isDefined() &&
            _big_blind.isDefined() &&
            _own_timebank.isDefined() &&
            _opp_timebank.isDefined() &&
            _own_increment.isDefined() &&
            _opp_increment.isDefined() &&
            _own_starting_time.isDefined() &&
            _opp_starting_time.isDefined() &&
            _own_starting_stack.isDefined() &&
            _opp_starting_stack.isDefined() &&
            _starting_player.isDefined();
    else if (!isActive())
        return 0;
    else
        return !getLastStreet().isActive();
}

int HandHistory :: isShowdown() const
{
    return isOwnCardsVisible() && isOppCardsVisible() && (_total_community_cards == 5);
}

/* Internal methods for updating hand */

void HandHistory :: addCommunityCard(Card card)
{
    if (_total_community_cards >= 5)
        throw HistoryError("Too many community cards");
    else
        _community_cards[_total_community_cards++] = card;
}

void HandHistory :: advanceStreetCounter()
{
    Street new_street;

    switch (lastStreet()) {
        
        case NULL_STREET :
            new_street = PREFLOP;
            break;

        case PREFLOP :
            new_street = FLOP;
            break;

        case FLOP :
            new_street = TURN;
            break;

        case TURN :
            new_street = RIVER;
            break;

        case RIVER :
            throw
               HistoryError("There are no streets past river"); 
            break;

        default:
            throw
                HistoryError("Unexpected error related to street counter");

    }

    _last_street = new_street;
}

void HandHistory :: checkForWinner()
{
    if (isActive() && (lastStreet() != NULL_STREET) && !getLastStreet().isActive()) {
        if (getLastStreet().isOwnFolded()) {
            int win_amount = std::min(getOwnPot(), getOppPot());
            declareWinner(OPP, -win_amount, win_amount);
        }
        else if (getLastStreet().isOppFolded()) {
            int win_amount = std::min(getOwnPot(), getOppPot());
            declareWinner(ME, win_amount, -win_amount);
        }
        else if (isShowdown()) {
            Card own_card1, own_card2, opp_card1, opp_card2;
            getOwnCards(own_card1, own_card2);
            getOppCards(opp_card1, opp_card2);

            Set own_set(getCommunityCards(), own_card1, own_card2);
            Set opp_set(getCommunityCards(), opp_card1, opp_card2);

            const char* own_set_string = getLogger().handValueToStr(own_set.getValue());
            const char* opp_set_string = getLogger().handValueToStr(opp_set.getValue());

            getLogger().log(L_DEBUG, "own hand strength = %s\n", own_set_string);
            getLogger().log(L_DEBUG, "opp hand strength = %s\n", opp_set_string);

            delete own_set_string;
            delete opp_set_string;

            if (own_set.getValue() > opp_set.getValue()) {
                int win_amount = std::min(getOwnPot(), getOppPot());
                declareWinner(ME, win_amount, -win_amount);
                getLogger().log(L_DEBUG, "You won %d chips\n", win_amount);
            }
            else if (own_set.getValue() < opp_set.getValue()) {
                int win_amount = std::min(getOwnPot(), getOppPot());
                declareWinner(OPP, -win_amount, win_amount);
                getLogger().log(L_DEBUG, "Opponent won %d chips\n", win_amount);
            }
            else {
                declareWinner(NEUTRAL, 0, 0);
                getLogger().log(L_DEBUG, "It's a draw\n");
            }
        }
    }
}

void HandHistory :: declareWinner(Side player, int own_win_amount, int opp_win_amount)
{
    _winner.set(player);
    _own_win_amount = own_win_amount;
    _opp_win_amount = opp_win_amount;
}

/* Methods for accessing internal variables */

const Logger& HandHistory :: getLogger() const
{
    return _logger;
}
