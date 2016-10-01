#ifndef __JUDGE__
#define __JUDGE__
#include "community.h"
#include "player_cards.h"
#include <utility>

/*Judge - a class which determines the the result of a game*/
class Judge {
public:
	virtual int verdict(const Community&, const PlayerCards&, const PlayerCards&) const = 0; //To get the result of a single scenario
	virtual void addScenario(const Community&, const PlayerCards&, const PlayerCards&) = 0; //Add a scenario
	virtual void reset() = 0; //Remove all scenarios
	virtual std::pair<float,float> verdict() const = 0; //Returns the result of all the stored scenarios
};
#endif