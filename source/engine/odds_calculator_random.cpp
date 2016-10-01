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

std::pair<float, float> OddsCalculatorRandom::odds(const CardContainer& com,const CardContainer& p1, const CardContainer& p2) {
    uint64_t victories = 0;
    uint64_t draws = 0;

	int cards[9]; // the cards present in com u p1 u p2
	
	int com_cards_number = com.getCards(cards);
	int p1_cards_number = p1.getCards(cards + com_cards_number);
	int p2_cards_number = p2.getCards(cards + com_cards_number + p1_cards_number);
	
	int cards_number = com_cards_number + p1_cards_number + p2_cards_number; //Total number of cards present
	
	
	int deck[52];
	for(int i=0; i!= 52; ++i) //initialize deck
		deck[i] = i; 	
	
	//Remove cards from the deck
	int deck_size = 52;
	int temp=0; 
	for(int i=0; i!= cards_number; ++i) {
		for(int j=0; j!=deck_size; ++j) {
			if(deck[j] == cards[i]) {
				temp = deck[deck_size - 1];
				deck[deck_size - 1] = cards[i];
				deck[j] = temp;
			}
		}
		--deck_size;
	}
	
	
    _judge.reset(); //Reset the judge
	
	//feed the judge with scenarios
	for(int i=0; i!= _trials; ++i) {
		_randomizer.randSubset(deck, deck_size, 9-cards_number);
		CardContainer temp_com = com;
		CardContainer temp_p1 = p1;
		CardContainer temp_p2 = p2;
		temp_com.addCard((Card*) deck, 5-com_cards_number);
		temp_p1.addCard((Card*) deck + com_cards_number, 2-p1_cards_number);
		temp_p2.addCard((Card*) deck + com_cards_number + p1_cards_number, 2- p2_cards_number);
		_judge.addScenario(temp_com, temp_p1, temp_p2);
	}
	
	//Return the result coming from the judge
	pair<float, float> ret = _judge.verdict();
	
	//reset the judge
	_judge.reset();
	return ret;
}

OddsCalculatorRandom :: OddsCalculatorRandom (Judge& ref) : _judge(ref),_confidence(0.99), _accuracy(0.001),_trials(0) {
    updateTrials();
}