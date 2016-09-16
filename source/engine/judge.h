#ifndef __JUDGE__
#define __JUDGE__
#include "community.h"
#include "player_cards.h"

/*Judge - a class which determines the score of a community + player hand */
class Judge {
public:
	virtual int verdict(const Community&, const PlayerCards&) = 0;
};
#endif