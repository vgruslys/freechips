#include "judge_q.h"
#include "qeval.h"
#include <utility>

JudgeQ :: JudgeQ (const QEval & ref):_scorer(ref) {
	_player1_scenarios.reserve(1000000);
	_player2_scenarios.reserve(1000000);
}

int JudgeQ :: verdict(const CardContainer& com, const CardContainer& p1, const CardContainer& p2) const {
	uint32_t p1_score = _scorer.score(com.getKey() + p1.getKey());
	uint32_t p2_score = _scorer.score(com.getKey() + p2.getKey());
	return p1_score > p2_score ? 1 : (p2_score > p1_score ? -1 : 0);
} 

void JudgeQ :: addScenario(const CardContainer& com, const CardContainer& p1, const CardContainer& p2) {
	_player1_scenarios.push_back(com.getKey() + p1.getKey());
	_player2_scenarios.push_back(com.getKey() + p2.getKey());
}

void JudgeQ :: reset() {
	_player1_scenarios.clear();
	_player2_scenarios.clear();
}

std::pair<float, float> JudgeQ :: verdict() const {
	uint64_t size = _player1_scenarios.size();
	uint32_t* _player1_scores = new uint32_t [size];
	uint32_t* _player2_scores = new uint32_t [size];
	_scorer.score(&(_player1_scenarios[0]), _player1_scores, size);
	_scorer.score(&(_player2_scenarios[0]), _player2_scores, size);
	
	uint64_t wins = 0;
	uint64_t draws = 0;
	for(int i=0; i!=size; ++i) {
		wins += (_player1_scores[i] > _player2_scores[i] ? 1 : 0);
		draws += (_player1_scores[i] == _player2_scores[i] ? 1 : 0);
	}
	delete [] _player1_scores;
	delete [] _player2_scores;
	return std::pair<float, float>((float) wins / (float) size, (float) draws / (float) size);
}
