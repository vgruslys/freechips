#include "card.h"
#include "community.h"
#include "key_numbers.h"

Community :: Community():_key(0), _coded_key(0), _card_map(new uint64_t [52]), _coded_card_map(new uint64_t [52]) {
	
	for(int i=0; i!=51; i++) {
		_coded_card_map[i] = (suit_numbers[i/13] << 32) + height_numbers[i % 13]; //first 32 bits contain the large key, 
		_card_map[i] = (1 << i); //the simple 64bit key
	}
	
}

void Community :: addCard(Card card) {
	_key += _card_map[card]; 
	_coded_key += _coded_card_map[card]; 
}
void Community :: removeCard(Card card) {
	_key -= _card_map[card]; 
	_coded_key -= _coded_card_map[card];

}

void Community :: reset() {
	_key = 0; 
	_coded_key = 0;
}

Community :: ~Community() {
	
}

uint64_t Community :: getKey() const {
	return _key;
}

uint64_t Community :: getCodedKey() const {
	return _coded_key;
}