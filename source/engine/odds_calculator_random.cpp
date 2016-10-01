#include "odds_calculator_random.h"
#include "random_deck_plain.h"
#include "sets.h"
#include <cmath>
#include <utility>
#include <iostream>
#include "stdint.h"

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

std::pair<float, float> OddsCalculatorRandom::odds(const CardContainer& com, const CardContainer& p1, const CardContainer& p2) {
    uint64_t victories = 0;
    uint64_t draws = 0;

	int cards[9]; // the cards present in com u p1 u p2
	
	int com_cards_number = com.getCards(cards);
	int p1_cards_number = p1.getCards(cards + com_cards_number);
	int p2_cards_number = p2.getCards(cards + com_cards_number + p1_cards_number);
	
	int cards_number = com_cards_number + p1_cards_number + p2_cards_number; //Total number of cards present
	
	for(int i=0; i!=cards_number; ++i) //Remove the cards present from the deck
		_rdeck.removeCard(cards[i]);
	
    _judge.reset(); //Reset the judge
	
	//TO DO
	return std::pair<float, float>(.0,.0);
}

OddsCalculatorRandom :: OddsCalculatorRandom (Judge& ref) : _judge(ref),_confidence(0.99), _accuracy(0.001),_trials(0) {
    updateTrials();
}