#ifndef __JUDGE_Q_
#define __JUDGE_Q_
#include "community.h"
#include "player_cards.h"
#include <utility>
#include "judge.h"
#include "qeval.h"
#include <vector>
#include "stdint.h"

/*JudgeQ - a Judge class which uses QEval*/
class JudgeQ : public Judge {
public:
	int verdict(const Community&, const PlayerCards&, const PlayerCards&) const final; //To get the result of a single scenario
	void addScenario(const Community&, const PlayerCards&, const PlayerCards&) final; //Add a scenario
	void reset() final; //Remove all scenarios
	std::pair<float,float> verdict() const final; //Returns the result of all the stored 
	
	JudgeQ (const QEval &);
private:
	const QEval& _scorer;
	std::vector<uint64_t> _player1_scenarios;
	std::vector<uint64_t> _player2_scenarios;
	
};
#endif