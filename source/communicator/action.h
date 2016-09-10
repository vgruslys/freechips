#ifndef __ACTION__
#define __ACTION__

#include "error.h"
#include "global_types.h"

class Action {
    public:
        Action(Side, ActionType, int, int); 
        Side getPlayer() const;
        ActionType getType() const;
        int getValue() const;
        int getTime() const;

    private:
        Side _player;
        ActionType _type;
        int _value; 
        int _time;
};

#endif
