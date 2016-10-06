#include "odds_calculator_random.h"
#include "sets.h"
#include <cmath>
#include <utility>
#include <iostream>
#include "stdint.h"
#include <bitset>
#include "ranvec1.h"
#include "randomizer.h"

#define  popc __builtin_popcountll

using namespace std;

inline void swap( uint32_t* array, uint32_t i1, uint32_t i2) {
	uint32_t temp = array[i1];
	array[i1] = array[i2];
	array[i2] = temp;
}

std::pair<float, float> OddsCalculatorRandom::odds(const CardContainer& community,const CardContainer& p1, const CardContainer& p2) {

	int community_cards[5];
	int p1_cards[2];
	int p2_cards[2];
	
	uint32_t community_size = community.getCards(community_cards);
	uint32_t p1_size = p1.getCards(p1_cards);
	uint32_t p2_size = p2.getCards(p2_cards);
	
	uint32_t community_left = 5 - community_size;
	uint32_t p1_left = 2 - p1_size;
	uint32_t p2_left = 2- p2_size;
	
	uint32_t left = community_left + p1_left + p2_left;
	
	uint32_t deck1[52];
	uint32_t deck2[52];
	uint32_t deck3[52];   
	uint32_t deck4[52];
	uint32_t deck5[52];
	uint32_t deck6[52];
	uint32_t deck7[52];
	uint32_t deck8[52];
		
	uint32_t deck_size = 52 - p1_size - p2_size - community_size;
	
	uint64_t present_cards = (p1.getKey() | p2.getKey() | community.getKey()) & 0xfffffffffffff;
	uint64_t one = 1;
	int index = 0;
	
	//Deck initialization
	for(int i=0; i!= 52; ++i){
		if(!(one & present_cards)) {
			deck1[index] = i;
			deck2[index] = i;
			deck3[index] = i;
			deck4[index] = i;
			deck5[index] = i;
			deck6[index] = i;
			deck7[index] = i;
			deck8[index] = i;
			++index;
		}
		one <<=1;
	}

	uint32_t random_card[5][8];

	CardContainer com1;
	CardContainer com2;
	CardContainer com3;
	CardContainer com4; 
	CardContainer com5;
	CardContainer com6;
	CardContainer com7;
	CardContainer com8;
	
	_judge.reset();
	
	for(uint32_t i=0; i!= _trials/8; ++i) {
	
		com1 = community;
		com2 = community;
		com3 = community;
		com4 = community;
		com5 = community; ;
		com6 = community;
		com7 = community;
		com8 = community;
		
		for(int i=0; i!= community_left; ++i) {
		_rgen.random8i(0,deck_size-1-i).store(random_card[i]);
		}
	
		for(int i=0; i!=community_left; ++i) {
			swap(deck1, random_card[i][0], deck_size - 1 - i);
			swap(deck2, random_card[i][1], deck_size - 1 - i);
			swap(deck3, random_card[i][2], deck_size - 1 - i);
			swap(deck4, random_card[i][3], deck_size - 1 - i);
			swap(deck5, random_card[i][4], deck_size - 1 - i);
			swap(deck6, random_card[i][5], deck_size - 1 - i);
			swap(deck7, random_card[i][6], deck_size - 1 - i);	
			swap(deck8, random_card[i][7], deck_size - 1 - i);	

			com1.addCard(deck1[deck_size - 1 - i]);
			com2.addCard(deck2[deck_size - 1 - i]);
			com3.addCard(deck3[deck_size - 1 - i]);
			com4.addCard(deck4[deck_size - 1 - i]);
			com5.addCard(deck5[deck_size - 1 - i]);
			com6.addCard(deck6[deck_size - 1 - i]);
			com7.addCard(deck7[deck_size - 1 - i]);
			com8.addCard(deck8[deck_size - 1 - i]);
		}


		_judge.addScenario(com1, p1, p2);
		_judge.addScenario(com2, p1, p2);
		_judge.addScenario(com3, p1, p2);
		_judge.addScenario(com4, p1, p2);
		_judge.addScenario(com5, p1, p2);
		_judge.addScenario(com6, p1, p2);
		_judge.addScenario(com7, p1, p2);
		_judge.addScenario(com8, p1, p2);

	}
	
	return _judge.verdict();
}

OddsCalculatorRandom :: OddsCalculatorRandom (Judge& ref) : _judge(ref),_trials(2000),_rgen(1) {
	_rgen.init(0);
}