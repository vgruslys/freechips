#include "logger.h"
#include <cstring>

Logger :: Logger(FILE* stream)
{
    _stream = stream;
    setShowAllLevels(0);
}

void Logger :: log(LogLevel level, const char* format, ...) const
{
    va_list args;
    va_start(args, format);
    if (getShowLevel(level)) {
        vfprintf(getStream(), format, args);
        fflush(getStream());
    }
    va_end(args);
}

void Logger :: setShowLevel(LogLevel level, int state)
{
    if (level == L_SYSTEM)
        _show_system = state;
    else if (level == L_DEBUG)
        _show_debug = state;
    else if (level == L_MUNDANE)
        _show_mundane = state;
    else
        throw LoggerError("Unrecognised log level");
}

int Logger :: getShowLevel(LogLevel level) const
{
    if (level == L_SYSTEM)
        return _show_system;
    else if (level == L_DEBUG)
        return _show_debug;
    else if (level == L_MUNDANE)
        return _show_debug;
    else
        throw LoggerError("Unrecognised log level");
}

void Logger :: setShowAllLevels(int state)
{
    setShowLevel(L_SYSTEM, state);
    setShowLevel(L_DEBUG, state);
    setShowLevel(L_MUNDANE, state);
}

FILE* Logger :: getStream() const
{
    return _stream;
}

const char* Logger :: commandToStr(Command command) const
{
    switch (command) {

        case PLAYER_ME :
            return "PLAYER_ME";
            break;
        case PLAYER_OPP :
            return "PLAYER_OPP";
            break;
        case SETTINGS :
            return "SETTINGS";
            break;
        case MATCH :
            return "MATCH";
            break;
        case ACTION :
            return "ACTION";
            break;
        case TIME_BANK :
            return "TIME_BANK";
            break;
        case TIME_PER_MOVE :
            return "TIME_PER_MOVE";
            break;
        case HANDS_PER_LEVEL :
            return "HANDS_PER_LEVEL";
            break;
        case STARTING_STACK :
            return "STARTING_STACK";
            break;
        case YOUR_BOT :
            return "YOUR_BOT";
            break;
        case ROUND :
            return "ROUND";
            break;
        case SMALL_BLIND :
            return "SMALL_BLIND";
            break;
        case BIG_BLIND :
            return "BIG_BLIND";
            break;
        case ON_BUTTON :
            return "ON_BUTTON";
            break;
        case TABLE :
            return "TABLE";
            break;
        case MAX_WIN_POT :
            return "MAX_WIN_POT";
            break;
        case AMOUNT_TO_CALL :
            return "AMOUNT_TO_CALL";
            break;
        case STACK :
            return "STACK";
            break;
        case POST :
            return "POST";
            break;
        case HAND :
            return "HAND";
            break;
        case FOLD :
            return "FOLD";
            break;
        case CHECK :
            return "CHECK";
            break;
        case CALL :
            return "CALL";
            break;
        case RAISE :
            return "RAISE";
            break;
        case WINS :
            return "WINS";
            break;

        default :
            return "???";
    }
}


const char* Logger :: streetToStr(Street street) const
{
    switch (street) {

        case NULL_STREET :
            return "NULL_STREET";
            break;
        case PREFLOP :
            return "PREFLOP";
            break;
        case FLOP :
            return "FLOP";
            break;
        case TURN :
            return "TURN";
            break;
        case RIVER :
            return "RIVER";
            break;
        default :
            return "???";
    }
}


const char* Logger :: actionTypeToStr(ActionType action_type) const
{
    switch (action_type) {

        case A_POST :
            return "A_POST";
            break;
        case A_CHECK :
            return "A_CHECK";
            break;
        case A_CALL :
            return "A_CALL";
            break;
        case A_RAISE :
            return "A_RAISE";
            break;
        case A_FOLD :
            return "A_FOLD";
            break;
        default :
            return "???";
    }
}


const char* Logger :: logLevelToStr(LogLevel log_level) const
{
    switch (log_level) {

        case L_SYSTEM :
            return "L_SYSTEM";
            break;
        case L_DEBUG :
            return "L_DEBUG";
            break;
        case L_MUNDANE :
            return "L_MUNDANE";
            break;
        default :
            return "???";
    }
}

const char* Logger :: sideToStr(Side side) const
{
    switch (side) {

        case ME :
            return "ME";
            break;
        case OPP :
            return "OPP";
            break;
        case NEUTRAL :
            return "NEUTRAL";
            break;
        default :
            return "???";
    }
}

const char* Logger :: handValueToStr(HandValue value) const
{
    char* string = new char[256];
    HandValue type = getHandValueType(value);

    switch (type) {
        
        case H_ROYAL_FLUSH :
            strcpy(string, "royal flush");
            break;

        case H_STRAIGHT_FLUSH :
            strcpy(string, "straight flush, *-high");
            string[16] = heightToChar(getHandValuePosition(value, 1));
            break;

        case H_FOUR_OF_A_KIND :
            strcpy(string, "four *s with *-kicker");
            string[5] = heightToChar(getHandValuePosition(value, 1));
            string[13] = heightToChar(getHandValuePosition(value, 2));
            break;

        case H_FULL_HOUSE :
            strcpy(string, "full house, *s with *s");
            string[13] = heightToChar(getHandValuePosition(value, 1));
            string[21] = heightToChar(getHandValuePosition(value, 2));
            break;

        case H_FLUSH :
            strcpy(string, "flush, *****");
            string[8] = heightToChar(getHandValuePosition(value, 1));
            string[9] = heightToChar(getHandValuePosition(value, 2));
            string[10] = heightToChar(getHandValuePosition(value, 3));
            string[11] = heightToChar(getHandValuePosition(value, 4));
            string[12] = heightToChar(getHandValuePosition(value, 5));
            break;

        case H_STRAIGHT :
            strcpy(string, "straight, *-high");
            string[10] = heightToChar(getHandValuePosition(value, 1));
            break;

        case H_THREE_OF_A_KIND :
            strcpy(string, "three *s with **-kickers");
            string[7] = heightToChar(getHandValuePosition(value, 1));
            string[15] = heightToChar(getHandValuePosition(value, 2));
            string[16] = heightToChar(getHandValuePosition(value, 3));
            break;

        case H_TWO_PAIR :
            strcpy(string, "two pairs, *s with *s, with *-kicker");
            string[11] = heightToChar(getHandValuePosition(value, 1));
            string[19] = heightToChar(getHandValuePosition(value, 2));
            string[28] = heightToChar(getHandValuePosition(value, 3));
            break;

        case H_PAIR :
            strcpy(string, "pair of *s with ***-kickers");
            string[8] = heightToChar(getHandValuePosition(value, 1));
            string[16] = heightToChar(getHandValuePosition(value, 2));
            string[17] = heightToChar(getHandValuePosition(value, 3));
            string[18] = heightToChar(getHandValuePosition(value, 4));
            break;

        case H_HIGH_CARD :
            strcpy(string, "*-high with ****");
            string[0] = heightToChar(getHandValuePosition(value, 1));
            string[12] = heightToChar(getHandValuePosition(value, 2));
            string[13] = heightToChar(getHandValuePosition(value, 3));
            string[14] = heightToChar(getHandValuePosition(value, 4));
            string[15] = heightToChar(getHandValuePosition(value, 5));
            break;

        default:
            throw LoggerError("Invalid hand value type");
    }

    return string;
}

