#include "global_types.h"

int streetCompare(Street street1, Street street2)
{
    switch (street1) {
        
        case NULL_STREET :
            if (street2 == NULL_STREET)
                return 0;
            else
                return 1;
            break;

        case PREFLOP :
            if (street2 == NULL_STREET)
                return -1;
            else if (street2 == PREFLOP)
                return 0;
            else
                return 1;
            break;

        case FLOP :
            if (street2 == NULL_STREET || street2 == PREFLOP)
                return -1;
            else if (street2 == FLOP)
                return 0;
            else
                return 1;
            break;

        case TURN :
            if (street2 == RIVER)
                return 1;
            else if (street2 == TURN)
                return 0;
            else
                return -1;
            break;

        case RIVER :
            if (street2 == RIVER)
                return 0;
            else
                return -1;
            break;
    }
}

Side changeSide(Side side)
{
    if (side == ME)
        return OPP;
    else if (side == OPP)
        return ME;
    else
        throw "Invalid side";
}
