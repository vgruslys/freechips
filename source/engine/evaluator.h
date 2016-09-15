#ifndef __EVALUATOR__
#define __EVALUATOR__
#include "community.h"
#include "player_cards.h"
/*Evaluator - a class which determines the score of a community + player hand */
class Evaluator {
public:
	virtual int evaluate(const Community&, const PlayerCards&);
	virtual int evaluate(const Community&, const PlayerCards&, const PlayerCards&);
};
#endif