#include "action.h"

Action :: Action(Side player, ActionType type, int value, int time)
{
    _player = player;
    _type = type;
    _value = value;
    _time = time;
} 

Side Action :: getPlayer() const
{
    return _player;
}

ActionType Action :: getType() const
{
    return _type;
}

int Action :: getValue() const
{
    return _value;
}

int Action :: getTime() const
{
    return _time;
}

