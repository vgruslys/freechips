#include "card.h"
#include "community.h"

Community :: Community():_key(0), _coded_key(0), _card_map(new uint64_t [52]), _coded_card_map(new uint64_t [52]) {
	//First, we define the special numbers - there are two sets, one for heights and one for suits
	//Maybe these numbers should be globally defined, or statically defined or whatever. I don't care
	uint64_t suit_numbers [4] = {6,38,35,1}; // these 4 numbers have the property that the sum of any 5, 6, or 7 of them 
											// with repeats allowed give a distinct sum
	uint64_t height_numbers [13] = {192812, 149924,36127,194147 ,65730,195103,174299,
										30521,185463,52988,90749,19256,65913};
										//These numbers have the property that the sum of any 5 of them with repeats allowed 
										 // give a distinct sum.
	for(int i=0; i!=51; i++) {
		_coded_card_map[i] = (suit_numbers[i/13] << 32) + height_numbers[i % 13]; //first 32 bits contain the large key, 
																				  //the last 32bits contain the small key
		_card_map[i] = (1 << i); //the simple 64bit key
	}
}

void Community :: addCard(Card card) {
	_key += _card_map[card]; 
	_coded_key += _card_map[card]; 
}
void Community :: removeCard(Card card) {
	_key -= _card_map[card]; 
	_coded_key -= _card_map[card];
}

void Community :: reset() {
	_key = 0; 
	_coded_key = 0;
}

Community :: ~Community() {
	delete [] suit_numbers;
	delete [] height_numbers;
}

Community :: getKey() const {
	return _key;
}

Community :: getCodedKey() const {
	return _coded_key;
}