#ifndef __GLOBAL_TYPES__
#define __GLOBAL_TYPES__

enum Command { PLAYER_ME, PLAYER_OPP, SETTINGS, MATCH, ACTION, TIME_BANK,
    TIME_PER_MOVE, HANDS_PER_LEVEL, STARTING_STACK, YOUR_BOT, ROUND,
    SMALL_BLIND, BIG_BLIND, ON_BUTTON, TABLE, MAX_WIN_POT, AMOUNT_TO_CALL,
    STACK, POST, HAND, FOLD, CHECK, CALL, RAISE, WINS };
         
enum Street { NULL_STREET, PREFLOP, FLOP, TURN, RIVER };
enum Side { ME, OPP, NEUTRAL };
enum ActionType { A_POST, A_CHECK, A_CALL, A_RAISE, A_FOLD };
enum LogLevel { L_SYSTEM, L_DEBUG, L_MUNDANE };

int streetCompare(Street, Street);
Side changeSide(Side);

#endif
