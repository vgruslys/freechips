#include "odds_calculator_random.h"
#include "random_deck_plain.h"
#include "sets.h"
#include <cmath>
#include <utility>
#include <iostream>
#include "stdint.h"
#include <bitset>
#include "ranvec1.h"
#include "mwc32_avx2.h"

using namespace std;
void OddsCalculatorRandom::setConfidence(float confidence) {
    _confidence = confidence;
    updateTrials();
} 

void OddsCalculatorRandom::setAccuracy(float accuracy) {
    _accuracy = accuracy;
    updateTrials();
}

void OddsCalculatorRandom::updateTrials()
{
     _trials = ceil(log(4.0/(1.0-_confidence))/(_accuracy*_accuracy));
}

std::pair<float, float> OddsCalculatorRandom::odds(const CardContainer& community,const CardContainer& p1, const CardContainer& p2) {

	int cards[9]; // the cards present in com u p1 u p2
	
	const int community_size = community.getCards(cards);
	const int p1_size = p1.getCards(cards);
	const int p2_size = p2.getCards(cards);
	
	const int size = community_size + p1_size + p2_size; //Total number of cards present


	uint64_t forbidden_cards = (p1.getKey() & 0x000fffffffffffff) + (p2.getKey() & 0x000fffffffffffff) 
								+ (community.getKey() & 0x000fffffffffffff);
	
    _judge.reset(); //Reset the judge
	
	
	CardContainer p1_copy = p1;
	CardContainer p2_copy = p2;
	CardContainer community_copy = community;
	
	uint8_t rand_card = 0;
	uint64_t forbidden_cards_copy = forbidden_cards;
	int community_counter =0;
	int p1_counter = 0;
	int p2_counter = 0;
	
	MWC32AVX2<160> rgen;
	
	uniform_int_distribution<int> distribution(0,51);
	
	//feed the judge with scenarios
	int random_card = 0;
	//cout << "Begin" << endl;
	uint64_t rand_card_64;
	for(int i=0; i!= _trials; ++i) {
		forbidden_cards_copy = forbidden_cards;
		p1_copy = p1;
		p2_copy = p2;
		community_copy = community;
		community_counter = 5-community_size;
		while(community_counter) {
			while( (forbidden_cards_copy | (rand_card_64 =(1ll << (rand_card = distribution(rgen))))) == forbidden_cards_copy);
			community_copy.addCard(rand_card);
			forbidden_cards_copy |= rand_card_64;
			community_counter--;
		}
		p1_counter = 2-p1_size;
		while(p1_counter) {
			while( (forbidden_cards_copy | (rand_card_64 =(1ll << (rand_card = distribution(rgen))))) == forbidden_cards_copy);
			p1_copy.addCard(rand_card);
			forbidden_cards_copy |= rand_card_64;
			p1_counter--;
		}
		p2_counter = 2-p2_size;
		while(p2_counter) {
			while( (forbidden_cards_copy | (rand_card_64 =(1ll << (rand_card = distribution(rgen))))) == forbidden_cards_copy);
			p2_copy.addCard(rand_card);
			forbidden_cards_copy |= rand_card_64;
			p2_counter--;
		}
		_judge.addScenario(community_copy, p1_copy, p2_copy);
	}
	
	//Return the result coming from the judge
	pair<float, float> ret = _judge.verdict();
	
	//reset the judge
	_judge.reset();
	return ret;
}

OddsCalculatorRandom :: OddsCalculatorRandom (Judge& ref) : _judge(ref),_confidence(0.99), _accuracy(0.001),_trials(0) {
    updateTrials();
	cout << "Trials : " << _trials << endl;
}