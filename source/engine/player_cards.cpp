#include "player_cards.h"
#include "stdint.h"
#include "key_numbers.h"

PlayerCards :: PlayerCards(): _key(0), _coded_key(0), _card_map(new uint64_t [52]), _coded_card_map(new uint64_t [52]) {
	for(int i=0; i!=51; i++) {
		_coded_card_map[i] = hand_height_numbers[i % 13]; //the coded unsuited key
		_card_map[i] = (1 << i); //the simple 64bit key
	}	
}

void PlayerCards :: addCard(Card card) {
	_key += _card_map[card];
	_coded_key += _coded_card_map[card];
} 


void PlayerCards :: removeCard(Card card) {
	_key -= _card_map[card];
	_coded_key -= _coded_card_map[card];
}

uint64_t PlayerCards :: getKey() const {return _key;}

uint64_t PlayerCards :: getCodedKey() const {return _coded_key;}