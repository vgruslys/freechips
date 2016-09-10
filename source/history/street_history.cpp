#include "street_history.h"

StreetHistory :: StreetHistory(const Logger& logger)
    : _logger(logger)
{
    _is_preflop = 0;
    _is_launched = 0;
    _own_folded = 0;
    _opp_folded = 0;
    _own_acted = 0;
    _opp_acted = 0;
    _own_posted = 0;
    _opp_posted = 0;
    _is_last_action_passive = 0;
}

void StreetHistory :: launch()
{
    if (isLaunched())
        throw HistoryError("Street is already launched");

    int ready_to_launch =
        _own_timebank.isDefined() &&
        _opp_timebank.isDefined() &&
        _own_increment.isDefined() &&
        _opp_increment.isDefined() &&
        _own_starting_time.isDefined() &&
        _opp_starting_time.isDefined() &&
        _own_starting_stack.isDefined() &&
        _opp_starting_stack.isDefined() &&
        _own_starting_pot.isDefined() &&
        _opp_starting_pot.isDefined() &&
        _starting_player.isDefined() &&
        _starting_min_raise.isDefined();

    if (!ready_to_launch)
        throw HistoryError("Street is not ready to launch");

    _own_stack = getOwnStartingStack();
    _opp_stack = getOppStartingStack();
    _own_pot = getOwnStartingPot();
    _opp_pot = getOppStartingPot();
    _own_time = getOwnStartingTime();
    _opp_time = getOppStartingTime();
    _min_raise = getStartingMinRaise();

    if (_is_preflop)
        _player = getStartingPlayer();
    else
        _player = changeSide(getStartingPlayer());

    _is_launched = 1;
    _is_active = 1;

    checkIfStillActive();
}

/* Methods for defining settings */

void StreetHistory :: defineOwnTimebank(int timebank)
{
    _own_timebank.set(timebank);
}

void StreetHistory :: defineOppTimebank(int timebank)
{
    _opp_timebank.set(timebank);
}

void StreetHistory :: defineOwnIncrement(int increment)
{
    _own_increment.set(increment);
}

void StreetHistory :: defineOppIncrement(int increment)
{
    _opp_increment.set(increment);
}

void StreetHistory :: defineOwnStartingTime(int time)
{
    _own_starting_time.set(time);
}

void StreetHistory :: defineOppStartingTime(int time)
{
    _opp_starting_time.set(time);
}

void StreetHistory :: defineOwnStartingStack(int stack)
{
    _own_starting_stack.set(stack);
}

void StreetHistory :: defineOppStartingStack(int stack)
{
    _opp_starting_stack.set(stack);
}

void StreetHistory :: defineOwnStartingPot(int pot)
{
    _own_starting_pot.set(pot);
}

void StreetHistory :: defineOppStartingPot(int pot)
{
    _opp_starting_pot.set(pot);
}

void StreetHistory :: defineStartingMinRaise(int amount)
{
    _starting_min_raise.set(amount);
}

void StreetHistory :: defineStartingPlayer(Side player)
{
    _starting_player.set(player);
}

void StreetHistory :: makeThisPreflop(int small_blind, int big_blind)
{
    _is_preflop = 1;
    _small_blind = small_blind;
    _big_blind = big_blind;
} 

/* Methods for accessing settings */

int StreetHistory :: getOwnTimebank() const
{
    return _own_timebank.get();
}

int StreetHistory :: getOppTimebank() const
{
    return _opp_timebank.get();
}

int StreetHistory :: getOwnIncrement() const
{
    return _own_increment.get();
}

int StreetHistory :: getOppIncrement() const
{
    return _opp_increment.get();
}

int StreetHistory :: getOwnStartingTime() const
{
    return _own_starting_time.get();
}

int StreetHistory :: getOppStartingTime() const
{
    return _opp_starting_time.get();
}

int StreetHistory :: getOwnStartingStack() const
{
    return _own_starting_stack.get();
}

int StreetHistory :: getOppStartingStack() const
{
    return _opp_starting_stack.get();
}

int StreetHistory :: getOwnStartingPot() const
{
    return _own_starting_pot.get();
}

int StreetHistory :: getOppStartingPot() const
{
    return _opp_starting_pot.get();
}

int StreetHistory :: getStartingMinRaise() const
{
    return _starting_min_raise.get();
}

Side StreetHistory :: getStartingPlayer() const
{
    return _starting_player.get();
}

int StreetHistory :: isPreflop() const
{
    return _is_preflop;
}

int StreetHistory :: getSmallBlind() const
{
    if (!isPreflop())
        throw HistoryError("This street is not preflop");
    else
        return _small_blind;
}

int StreetHistory :: getBigBlind() const
{
    if (!isPreflop())
        throw HistoryError("This street is not preflop");
    else
        return _big_blind;
}

/* Methods for accessing current parameters */

int StreetHistory :: getPot() const
{
    return getOwnPot() + getOppPot();
}

int StreetHistory :: getOwnPot() const
{
    return _own_pot;
}

int StreetHistory :: getOppPot() const
{
    return _opp_pot;
}

int StreetHistory :: getOwnStack() const
{
    return _own_stack;
}

int StreetHistory :: getOppStack() const
{
    return _opp_stack;
}

Side StreetHistory :: getPlayer() const
{
    return _player;
}

int StreetHistory :: getAmountToCall() const
{
    Side player = getPlayer();

    if (player == ME) {
        int difference = std::max(getOppPot() - getOwnPot(), 0);
        int room = getOwnStack();
        return std::min(room, difference);
    }
    else if (player == OPP) {
        int  difference = std::max(getOwnPot() - getOppPot(), 0);
        int room = getOppStack();
        return std::min(room, difference);
    }
    else
        throw HistoryError("Unexpected error related to next player to move");
}

int StreetHistory :: getOwnActed() const
{
    return _own_acted;
}

int StreetHistory :: getOppActed() const
{
    return _opp_acted;
}

int StreetHistory :: getOwnPosted() const
{
    return _own_posted;
}

int StreetHistory :: getOppPosted() const
{
    return _opp_posted;
}

int StreetHistory :: getOwnTime() const
{
    return _own_time;
}

int StreetHistory :: getOppTime() const
{
    return _opp_time;
}

int StreetHistory :: getMinRaise() const
{
    return _min_raise;
}

/* Methods for confirming properties of street */

void StreetHistory :: confirmPot(int pot) const
{
    int own_pot = getOwnPot();
    int opp_pot = getOppPot();
    int own_total = own_pot + getOwnStack();
    int opp_total = opp_pot + getOppStack();
    int actual_pot = std::min(own_pot, opp_total) + std::min(opp_pot, own_total);

    if (pot != actual_pot)
        throw HistoryError("Inconsistent pot size");

}

void StreetHistory :: confirmAmountToCall(int amount) const
{
    if (amount != getAmountToCall())
        throw HistoryError("Inconsistent amount to call");
}

void StreetHistory :: confirmActionRequired(Side player, int time) const
{
    if ((!isActive()) || (getPlayer() != player))
        throw HistoryError("This player should not act now");

    //confirmTime(player, time);
}

void StreetHistory :: confirmOwnStack(int stack) const
{
    if (stack != getOwnStack())
        throw HistoryError("Inconsistent size of own stack");
}

void StreetHistory :: confirmOppStack(int stack) const
{
    if (stack != getOppStack())
        throw HistoryError("Inconsistent size of opponent stack");
}

void StreetHistory :: confirmTime(Side player, int time) const
{
    int actual_time;
    if (player == ME)
        actual_time = getOwnTime();
    else
        actual_time = getOppTime();

    if (actual_time - time > 100 || actual_time - time < -100)
        throw HistoryError("Inconsistent remaining time");
}

/* Methods for accessing winner of street */

int StreetHistory :: isActive() const
{
    return _is_active;
}

int StreetHistory :: isLaunched() const
{
    return _is_launched;
}

int StreetHistory :: isOwnFolded() const
{
    return _own_folded;
}

int StreetHistory :: isOppFolded() const
{
    return _opp_folded;
}

int StreetHistory :: isOwnAllIn() const
{
    return getOwnStack() == 0;
}

int StreetHistory :: isOppAllIn() const
{
    return getOppStack() == 0;
}

/* Methods for making actions */

void StreetHistory :: makeAction(Side player, ActionType type, int amount, int time)
{
    if (!isActive())
        throw HistoryError("Actions cannot be made because the street is not active");

    if (player != getPlayer())
        throw HistoryError("The other player is expected to act");

    switch (type) {
        
        case A_POST :
            post(player, amount);
            break;
        case A_CHECK :
            check(player);
            break;
        case A_CALL :
            call(player);
            break;
        case A_RAISE :
            raise(player, amount);
            break;
        case A_FOLD :
            fold(player);
            break;
    }

    useTime(player, time);
    _player = changeSide(getPlayer());

    checkIfStillActive();
}

/* Methods for betting actions */

void StreetHistory :: post(Side player, int amount)
{
    if (!isPreflop())
        throw HistoryError("Blinds can only be posted in preflop");

    int blind;
    if (player == getStartingPlayer())
        blind = getSmallBlind();
    else
        blind = getBigBlind();

    if (player == ME) {

        if (getOwnPosted())
            throw HistoryError("Blind was already posted");

        if (amount != std::min(getOwnStack(), blind))
            throw HistoryError("Inconsistent size of blind");

        payOwn(amount);
        _own_posted = 1;

    }
    else {

        if (getOppPosted())
            throw HistoryError("Blind was already posted");

        if (amount != std::min(getOppStack(), blind))
            throw HistoryError("Inconsistent size of blind");

        payOpp(amount);
        _opp_posted = 1;
    
    }
}

void StreetHistory :: raise(Side player, int amount)
{
    if (isPreflop() && !(getOwnPosted() && getOppPosted()))
        throw HistoryError("Blinds have to be posted first");

    if (player == ME) {

        if (amount < std::min(getMinRaise(), getOwnStack() - getAmountToCall()))
            throw HistoryError("Raise is lower than min-raise");

        if (isOppAllIn())
            throw HistoryError("Cannot raise an opponent who is all-in");

        payOwn(amount + getAmountToCall());
        _is_last_action_passive = 0;
        _own_acted = 1;

        updateMinRaise(amount);

    }
    else {

        if (amount < std::min(getMinRaise(), getOppStack() - getAmountToCall()))
            throw HistoryError("Raise is lower than min-raise");

        if (isOwnAllIn())
            throw HistoryError("Cannot raise an opponent who is all-in");

        payOwn(amount + getAmountToCall());
        _is_last_action_passive = 0;
        _opp_acted = 1;

        updateMinRaise(amount);

    }
}

void StreetHistory :: call(Side player)
{
    if (isPreflop() && !(getOwnPosted() && getOppPosted()))
        throw HistoryError("Blinds have to be posted first");

    if (player == ME) {

        if (getAmountToCall() == 0)
            throw HistoryError("Illegal call: should check instead");

        payOwn(getAmountToCall());
        _is_last_action_passive = 1;
        _own_acted = 1;

        
    }
    else {

        if (getAmountToCall() == 0)
            throw HistoryError("Illegal call: should check instead");

        payOpp(getAmountToCall());
        _is_last_action_passive = 1;
        _opp_acted = 1;

    }
}

void StreetHistory :: check (Side player)
{
    if (isPreflop() && !(getOwnPosted() && getOppPosted()))
        throw HistoryError("Blinds have to be posted first");

    if (getAmountToCall() > 0)
        throw HistoryError("Illegal check: should call instead");

    if (player == ME) {
        _is_last_action_passive = 1;
        _own_acted = 1;
    }
    else {
        _is_last_action_passive = 1;
        _opp_acted = 1;
    }
}

void StreetHistory :: fold (Side player)
{
    if (isPreflop() && !(getOwnPosted() && getOppPosted()))
        throw HistoryError("Blinds have to be posted first");

    if (player == ME) {
        _own_folded = 1;
        _own_acted = 1;
    }
    else {
        _opp_folded = 1;
        _opp_acted = 1;
    }
}

void StreetHistory :: useTime(Side player, int time)
{
    if (player == ME)
        _own_time = std::min(getOwnTime() + getOwnIncrement() - time, getOwnTimebank());
    else
        _opp_time = std::min(getOppTime() + getOppIncrement() - time, getOppTimebank());
}

void StreetHistory :: payOwn(int amount)
{
    if (amount <= 0)
        throw HistoryError("Betting a non-positive amount");

    if (amount > getOwnStack())
        throw HistoryError("Betting more chips than there is in the stack");

    _own_stack -= amount;
    _own_pot += amount;
}

void StreetHistory :: payOpp(int amount)
{
    if (amount <= 0)
        throw HistoryError("Betting a non-positive amount");

    if (amount > getOppStack())
        throw HistoryError("Betting more chips than there is in the stack");

    _opp_stack -= amount;
    _opp_pot += amount;
}

void StreetHistory :: updateMinRaise(int amount)
{
    _min_raise = std::max(getMinRaise(), amount);
}

/* Methods for updating status of street */

int StreetHistory :: isLastActionPassive() const
{
    return _is_last_action_passive;
}

void StreetHistory :: checkIfStillActive()
{
    int someone_folded = isOwnFolded() || isOppFolded();
    int own_all_in = isOwnAllIn() && (getOppPot() >= getOwnPot());
    int opp_all_in = isOppAllIn() && (getOwnPot() >= getOppPot());
    int call_happened = isLastActionPassive() && getOwnActed() && getOppActed();

    if (someone_folded || own_all_in || opp_all_in || call_happened)
        _is_active = 0;
}
