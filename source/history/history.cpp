#include "history.h"

History :: History(const Logger& logger)
    : _logger(logger)
{
}

/* Methods for defining settings */

void History :: defineStartingSmallBlind(int blind)
{
    _starting_small_blind.set(blind);
    updateBlinds();
}

void History :: defineStartingBigBlind(int blind)
{
    _starting_big_blind.set(blind);
    updateBlinds();
}

void History :: defineHandsUntilChange(int hands)
{
    _hands_until_change.set(hands);
    updateBlinds();
}

void History :: defineOwnTimebank(int timebank)
{
    _own_timebank.set(timebank);
}

void History :: defineOppTimebank(int timebank)
{
    _opp_timebank.set(timebank);
}

void History :: defineOwnIncrement(int increment)
{
    _own_increment.set(increment);
}

void History :: defineOppIncrement(int increment)
{
    _opp_increment.set(increment);
}

void History :: defineOwnStartingStack(int stack)
{
    _own_starting_stack.set(stack);
}

void History :: defineOppStartingStack(int stack)
{
    _opp_starting_stack.set(stack);
}

void History :: defineStartingPlayer(Side player)
{
    _starting_player.set(player);
}

/* Methods for accessing settings */

int History :: getStartingSmallBlind() const
{
    return _starting_small_blind.get();
}

int History :: getStartingBigBlind() const
{
    return _starting_big_blind.get();
}

int History :: getHandsUntilChange() const
{
    return _hands_until_change.get();
}

int History :: getOwnTimebank() const
{
    return _own_increment.get();
}

int History :: getOppTimebank() const
{
    return _opp_increment.get();
}

int History :: getOwnIncrement() const
{
    return _own_increment.get();
}

int History :: getOppIncrement() const
{
    return _opp_increment.get();
}

int History :: getOwnStartingStack() const
{
    return _own_starting_stack.get();
}

int History :: getOppStartingStack() const
{
    return _opp_starting_stack.get();
}

Side History :: getStartingPlayer() const
{
    return _starting_player.get();
}

/* Methods for confirming properties of hand */

void History :: confirmSmallBlind(int blind) const
{
    getLastHand().confirmSmallBlind(blind);
}

void History :: confirmBigBlind(int blind) const
{
    getLastHand().confirmBigBlind(blind);
}

void History :: confirmStartingPlayer(Side player) const
{
    getLastHand().confirmStartingPlayer(player);
}

void History :: confirmWinner(Side player, int amount) const
{
    getLastHand().confirmWinner(player, amount);
}

/* Methods for confirming properties of street */

void History :: confirmPot(int pot) const
{
    getLastHand().confirmPot(pot);
}

void History :: confirmAmountToCall(int amount) const
{
    getLastHand().confirmAmountToCall(amount);
}

void History :: confirmActionRequired(Side player, int time) const
{
    getLastHand().confirmActionRequired(player, time);
}

void History :: confirmOwnStack(int stack) const
{
    getLastHand().confirmOwnStack(stack);
}

void History :: confirmOppStack(int stack) const
{
    getLastHand().confirmOppStack(stack);
}

/* Methods for updating hand */

void History :: startNewHand(int number)
{
    if (!isReadyToStartNewHand())
        throw HistoryError("Not ready to start new hand");

    if (number != totalHands()+1)
        throw HistoryError("Unexpected id number for new hand");

    int small_blind, big_blind;
    int own_timebank, opp_timebank;
    int own_increment, opp_increment;
    int own_time, opp_time;
    int own_stack, opp_stack;
    Side starting_player;

    computeNewBlinds(small_blind, big_blind);
    own_timebank = getOwnTimebank();
    opp_timebank = getOppTimebank();
    own_increment = getOwnIncrement();
    opp_increment = getOppIncrement();
    own_time = computeNewOwnTime();
    opp_time = computeNewOppTime();
    own_stack = computeNewOwnStack();
    opp_stack = computeNewOppStack();
    starting_player = computeNewPlayer();
    
    _hands.push_back(HandHistory(getLogger()));
    
    getLastHandToModify().defineSmallBlind(small_blind);
    getLastHandToModify().defineBigBlind(big_blind);
    getLastHandToModify().defineOwnTimebank(own_timebank);
    getLastHandToModify().defineOppTimebank(opp_timebank);
    getLastHandToModify().defineOwnIncrement(own_increment);
    getLastHandToModify().defineOppIncrement(opp_increment);
    getLastHandToModify().defineOwnStartingTime(own_time);
    getLastHandToModify().defineOppStartingTime(opp_time);
    getLastHandToModify().defineOwnStartingStack(own_stack);
    getLastHandToModify().defineOppStartingStack(opp_stack);
    getLastHandToModify().defineStartingPlayer(starting_player);

    getLastHandToModify().launch();

}

void History :: setCommunityCards(int total_cards, Card* cards)
{
    getLastHandToModify().setCommunityCards(total_cards, cards);
}

void History :: setOwnCards(Card card1, Card card2)
{
    getLastHandToModify().setOwnCards(card1, card2);
}

void History :: setOppCards(Card card1, Card card2)
{
    getLastHandToModify().setOppCards(card1, card2);
}

/* Methods for updating street */

void History :: startNewStreet()
{
    getLastHandToModify().startNewStreet();
}

void History :: makeAction(Side player, ActionType type, int value, int time)
{
    getLastHandToModify().makeAction(player, type, value, time);
}

/* Methods for getting information about hands/streets */

int History :: totalHands() const
{
    return _hands.size();
}

const HandHistory& History :: getHand(int hand) const
{
    if (hand < 0 || hand >= totalHands())
        throw HistoryError("Trying to access hand with invalid number");
    else
        return _hands[hand];
}

const HandHistory& History :: getLastHand() const
{
    if (totalHands() == 0)
        throw HistoryError("There are no hands");
    else
        return _hands[totalHands()-1];
}

const StreetHistory& History :: getLastStreet() const
{
    return getLastHand().getLastStreet();
}

HandHistory& History :: getHandToModify(int hand)
{
    if (hand < 0 || hand >= totalHands())
        throw HistoryError("Trying to access hand with invalid number");
    else
        return _hands[hand];
}

HandHistory& History :: getLastHandToModify()
{
    if (totalHands() == 0)
        throw HistoryError("There are no hands");
    else
        return _hands[totalHands()-1];
}

StreetHistory& History :: getLastStreetToModify()
{
    return getLastHandToModify().getLastStreetToModify();
}

/* Methods for controlling the status of history */

void History :: updateBlinds()
{
    if ((!_blinds_tracker.isInitialised()) && 
        _starting_big_blind.isDefined() &&
        _starting_small_blind.isDefined())
        
        initialiseBlindsTracker();
}

void History :: initialiseBlindsTracker()
{
    int small_blind = _starting_small_blind.get();
    int big_blind = _starting_big_blind.get();
    int hands_until_change = _hands_until_change.get();
    _blinds_tracker.initialise(small_blind, big_blind, hands_until_change);
}

/* Methods for tracking if the commands come in the right order */

int History :: isReadyToStartNewHand() const
{
    if (totalHands() == 0) {
        return _starting_small_blind.isDefined() &&
               _starting_big_blind.isDefined() &&
               _hands_until_change.isDefined() &&
               _blinds_tracker.isInitialised() &&
               _own_timebank.isDefined() &&
               _opp_timebank.isDefined() &&
               _own_increment.isDefined() &&
               _opp_increment.isDefined() &&
               _own_starting_stack.isDefined() &&
               _opp_starting_stack.isDefined() &&
               _starting_player.isDefined();
    }
    else
        return !getLastHand().isActive();
}

/* Methods for creating new hand */

Side History :: computeNewPlayer() const
{
    if (totalHands() == 0)
        return getStartingPlayer();
    else
        return changeSide(getLastHand().getStartingPlayer());
}

void History :: computeNewBlinds(int& small_blind, int& big_blind)
{
    small_blind = getBlindsTracker().getSmallBlind();
    big_blind = getBlindsTracker().getBigBlind();
    getBlindsTracker().advance();
}

int History :: computeNewOwnTime() const
{
    //---------- TODO when doing time -----------
    return getOwnTimebank();
}

int History :: computeNewOppTime() const
{
    //---------- TODO when doing time -----------
    return getOppTimebank();
}

int History :: computeNewOwnStack() const
{
    if (totalHands() == 0)
        return getOwnStartingStack();
    else
        return getLastHand().getOwnStack();
}

int History :: computeNewOppStack() const
{
    if (totalHands() == 0)
        return getOppStartingStack();
    else
        return getLastHand().getOppStack();
}

/* Methods for accessing internal variables */

const Logger& History :: getLogger() const
{
    return _logger;
}

BlindsTracker& History :: getBlindsTracker()
{
    return _blinds_tracker;
}


