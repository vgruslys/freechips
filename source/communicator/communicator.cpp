#include "communicator.h"

void Communicator :: check() 
{
    fprintf(getStreamOut(), "check 0\n");
    fflush(getStreamOut());

    int time = getCurrTime();
    setResponseTime(getCurrTime());
}

void Communicator :: call() 
{
    fprintf(getStreamOut(), "call 0\n");
    fflush(getStreamOut());

    int time = getCurrTime();
    setResponseTime(getCurrTime());
}

void Communicator :: fold() 
{
    fprintf(getStreamOut(), "fold 0\n");
    fflush(getStreamOut());

    int time = getCurrTime();
    setResponseTime(getCurrTime());
}

void Communicator :: raise(int value)
{
    fprintf(getStreamOut(), "raise %d\n", value);
    fflush(getStreamOut());
    
    int time = getCurrTime();
    setResponseTime(getCurrTime());
}

int Communicator :: readInt() const
{
    int result;
    fscanf(getStreamIn(), " %d", &result);
    return result;
}

char* Communicator :: readStr() const
{
    char* result = new char[256];
    fscanf(getStreamIn(), " %s", result);
    return result;
}

void Communicator :: dumpInt() const
{
    fscanf(getStreamIn(), " %*d");
}

void Communicator :: dumpStr() const
{
    fscanf(getStreamIn(), " %*s");
}

long long Communicator :: getCurrTime() const
{
    timeval time;
    gettimeofday(&time, NULL);
    long long formatted_time = (long long) time.tv_sec * 1000 + ((long long) time.tv_usec / 1000);
    return formatted_time;
}

void Communicator :: restartTimer()
{
    setRequestTime(getCurrTime());
}

int Communicator :: readTimer()
{
    long long old_time = getRequestTime();
    long long new_time = getCurrTime();
    return (int) (new_time - old_time);
}

Communicator :: Communicator(FILE* stream_in, FILE* stream_out, History& history, const Logger& logger)
    : _history(history),
      _logger(logger)
{
    _stream_in = stream_in;
    _stream_out = stream_out;

    _own_name_saved = 0;
    _opp_name_saved = 0;
}

FILE* Communicator :: getStreamIn() const
{
    return _stream_in;
}

FILE* Communicator :: getStreamOut() const
{
    return _stream_out;
}

void Communicator :: setHistory(History& history)
{
    throw "This method is corrupted";
}

History& Communicator :: getHistory()
{
    return _history;
}

const Logger& Communicator :: getLogger() const
{
    return _logger;
}

int Communicator :: hasOwnName() const
{
    return _own_name_saved;
}

int Communicator :: hasOppName() const
{
    return _opp_name_saved;
}

void Communicator :: setOwnName(const char* name)
{
    if (hasOwnName())
        throw CommunicatorError("Own name already set");
    else {
        strcpy(_own_name, name);
        _own_name_saved = 1;
    }
}

const char* Communicator :: getOwnName() const
{
    if (!hasOwnName())
        throw CommunicatorError("Own name not yet set");
    else
        return _own_name;
}

void Communicator :: setOppName(const char* name)
{
    if (hasOppName())
        throw CommunicatorError("Opponent name already set");
    else {
        strcpy(_opp_name, name);
        _opp_name_saved = 1;
    }
}

const char* Communicator :: getOppName() const
{
    if (!hasOppName())
        throw CommunicatorError("Opponent name not yet set");
    else
        return _opp_name;
}

void Communicator :: setRequestTime(long long time)
{
    _request_time = time;
}

long long Communicator :: getRequestTime() const
{
    return _request_time;
}

void Communicator :: setResponseTime(long long time)
{
    _response_time = time;
}

long long Communicator :: getResponseTime() const
{
    return _response_time;
}

void Communicator :: awaitInstructions()
{
    Command command1, command2;
    CommandPair header;
    int time_for_action = -1;

    do {
        header = readHeader();
        command1 = header.first;
        command2 = header.second;
        
        try {
            time_for_action = doCommand(command1, command2);
        }
        catch (CardError e) {
            getLogger().log(L_SYSTEM, "\nCARD EXCEPTION!!! %s\n", e.what());
        }
        catch (HistoryError e) {
            getLogger().log(L_SYSTEM, "\nHISTORY EXCEPTION!!! %s\n", e.what());
        }
        catch (CommunicatorError e) {
            getLogger().log(L_SYSTEM, "\nCOMMUNICATOR EXCEPTION!!! %s\n", e.what());
        }
        catch (LoggerError e) {
            getLogger().log(L_SYSTEM, "\nLOGGER EXCEPTION!!! %s\n", e.what());
        }
        catch (DefinableError e) {
            getLogger().log(L_SYSTEM, "\nLOGGER EXCEPTION!!! %s\n", e.what());
        }
        catch (BlindsError e) {
            getLogger().log(L_SYSTEM, "\nLOGGER EXCEPTION!!! %s\n", e.what());
        }
    } while (time_for_action == -1);
}

CommandPair Communicator :: readHeader() const
{
    char* string1;
    char* string2;
    Command command1, command2;

    string1 = readStr();
    string2 = readStr();

    getLogger().log(L_DEBUG, "Reading command from the server: '%s %s'\n", string1, string2);

    if (hasOwnName() && std::strcmp(string1, getOwnName()) == 0)
        command1 = PLAYER_ME;
    else if (hasOppName() && std::strcmp(string1, getOppName()) == 0)
        command1 = PLAYER_OPP;
    else if (std::strcmp(string1, "Settings") == 0)
        command1 = SETTINGS;
    else if (std::strcmp(string1, "Match") == 0)
        command1 = MATCH;
    else if (std::strcmp(string1, "Action") == 0)
        command1 = ACTION;
    else
        throw CommunicatorError("Unrecognised first command");

    if (hasOwnName() && std::strcmp(string2, getOwnName()) == 0)
        command2 = PLAYER_ME;
    else if (hasOppName() && std::strcmp(string2, getOppName()) == 0)
        command2 = PLAYER_OPP;
    else if (std::strcmp(string2, "timebank") == 0)
        command2 = TIME_BANK;
    else if (std::strcmp(string2, "time_per_move") == 0)
        command2 = TIME_PER_MOVE;
    else if (std::strcmp(string2, "hands_per_level") == 0)
        command2 = HANDS_PER_LEVEL;
    else if (std::strcmp(string2, "starting_stack") == 0)
        command2 = STARTING_STACK;
    else if (std::strcmp(string2, "your_bot") == 0)
        command2 = YOUR_BOT;
    else if (std::strcmp(string2, "round") == 0)
        command2 = ROUND;
    else if (std::strcmp(string2, "small_blind") == 0 || std::strcmp(string2, "smallBlind") == 0)
        command2 = SMALL_BLIND;
    else if (std::strcmp(string2, "big_blind") == 0 || std::strcmp(string2, "bigBlind") == 0)
        command2 = BIG_BLIND;
    else if (std::strcmp(string2, "on_button") == 0 || std::strcmp(string2, "onButton") == 0)
        command2 = ON_BUTTON;
    else if (std::strcmp(string2, "table") == 0)
        command2 = TABLE;
    else if (std::strcmp(string2, "max_win_pot") == 0 || std::strcmp(string2, "maxWinPot") == 0)
        command2 = MAX_WIN_POT;
    else if (std::strcmp(string2, "amount_to_call") == 0 || std::strcmp(string2, "amountToCall") == 0)
        command2 = AMOUNT_TO_CALL;
    else if (std::strcmp(string2, "stack") == 0)
        command2 = STACK;
    else if (std::strcmp(string2, "hand") == 0)
        command2 = HAND;
    else if (std::strcmp(string2, "fold") == 0)
        command2 = FOLD;
    else if (std::strcmp(string2, "check") == 0)
        command2 = CHECK;
    else if (std::strcmp(string2, "call") == 0)
        command2 = CALL;
    else if (std::strcmp(string2, "raise") == 0)
        command2 = RAISE;
    else if (std::strcmp(string2, "wins") == 0)
        command2 = WINS;
    else if (std::strcmp(string2, "post") == 0)
        command2 = POST;
    else
        throw CommunicatorError("Unrecognised second command");

    delete string1;
    delete string2;

    return std::make_pair<Command,Command> (command1,command2);
}

int Communicator :: doCommand(Command command1, Command command2)
{
    getLogger().log(L_DEBUG, "Doing command...\n");

    if (command1 == SETTINGS) {
        if (command2 == TIME_BANK) {
            int timebank = readInt();
            getHistory().defineOwnTimebank(timebank);
            getHistory().defineOppTimebank(timebank);
        }
        else if (command2 == TIME_PER_MOVE) {
            int increment = readInt();
            getHistory().defineOwnIncrement(increment);
            getHistory().defineOppIncrement(increment);
        }
        else if (command2 == HANDS_PER_LEVEL) {
            int hands = readInt();
            getHistory().defineHandsUntilChange(hands);
            getHistory().defineStartingSmallBlind(10); // Terrible hack
            getHistory().defineStartingBigBlind(20);   //
        }
        else if (command2 == STARTING_STACK) {
            int stack = readInt();
            getHistory().defineOwnStartingStack(stack);
            getHistory().defineOppStartingStack(stack);
        }
        else if (command2 == YOUR_BOT) {
            char* name = readStr();
            if (std::strcmp(name, "player1") == 0) {
                setOwnName("player1");
                setOppName("player2");
                getHistory().defineStartingPlayer(OPP); // Terrible hack
            }
            else if (std::strcmp(name, "player2") == 0) {
                setOwnName("player2");
                setOppName("player1");
                getHistory().defineStartingPlayer(ME); // Terrible hack
            }
            else
                throw CommunicatorError("Invalid player name");
            delete name;
        }
        else
            throw CommunicatorError("Invalid second command for Settings");
    }

    else if (command1 == MATCH) {
        if (command2 == ROUND) {
            int hand = readInt();

            getLogger().log(L_DEBUG, "Starting hand #%d\n", hand);

            getHistory().startNewHand(hand);
        } 
        else if (command2 == SMALL_BLIND) {
            int small_blind = readInt();
            getHistory().confirmSmallBlind(small_blind);
        }
        else if (command2 == BIG_BLIND) {
            int big_blind = readInt();
            getHistory().confirmBigBlind(big_blind);
        }
        else if (command2 == ON_BUTTON) {
            char* name = readStr();

            Side player;
            if (std::strcmp(name, getOwnName()) == 0)
                player = ME;
            else if (std::strcmp(name, getOppName()) == 0)
                player = OPP;
            else
                throw CommunicatorError("Invalid player name");

            delete name;
            getHistory().confirmStartingPlayer(player);
        }
        else if (command2 == TABLE) {
            char* table = readStr();

            int total_cards;
            Card cards[5];
            
            total_cards = parseCards(table, cards);
            delete table;

            getHistory().startNewStreet();
            getHistory().setCommunityCards(total_cards, cards);

        }
        else if (command2 == MAX_WIN_POT) {
            int pot = readInt();
            getHistory().confirmPot(pot);
        }
        else if (command2 == AMOUNT_TO_CALL) {
            int amount = readInt();
            getHistory().confirmAmountToCall(amount);
        }
        else
            throw CommunicatorError("Invalid second command for Match");
    }

    else if (command1 == ACTION) {
        restartTimer();
        if (command2 == PLAYER_ME) {
            int time = readInt();
            getHistory().confirmActionRequired(ME, time);
            return time;
        }
        else
            throw CommunicatorError("Invalid second command for Action");
    }

    else if (command1 == PLAYER_ME || command1 == PLAYER_OPP) {
        Side player;
        if (command1 == PLAYER_ME)
            player = ME;
        else
            player = OPP;

        if (command2 == STACK) {
            int stack = readInt();
            if (player == ME)
                getHistory().confirmOwnStack(stack);
            else
                getHistory().confirmOppStack(stack);
        }
        else if (command2 == HAND) {
            char* string = readStr();

            int total_cards;
            Card cards[5];
            total_cards = parseCards(string, cards);
            delete string;

            if(total_cards != 2)
                throw CommunicatorError("There should be exactly two cards in a player's hand");
            else {
                if (player == ME)
                    getHistory().setOwnCards(cards[0], cards[1]);
                else
                    getHistory().setOppCards(cards[0], cards[1]);
            }
        }
        else if (command2 == WINS) {
            int value = readInt();
            getHistory().confirmWinner(player, value);
        }
        else if (command2 == POST || command2 == FOLD || command2 == CHECK || command2 == CALL || command2 == RAISE) {
            int value = readInt();
            int time = readTimer();
            ActionType type;
            if (command2 == POST)
                type = A_POST;
            else if (command2 == CHECK)
                type = A_CHECK;
            else if (command2 == CALL)
                type = A_CALL;
            else if (command2 == RAISE)
                type = A_RAISE;
            else if (command2 == FOLD)
                type = A_FOLD;
            else
                throw CommunicatorError("Unexpected error regarding action types");

            getHistory().makeAction(player, type, value, time);
        }
        else
            throw CommunicatorError("Invalid second command following a player's name");
    }

    else
        throw CommunicatorError("Invalid first command");

    return -1;
}
