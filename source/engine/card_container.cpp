#include "card.h"
#include "card_container.h"
#include "stdint.h"

const uint64_t CardContainer :: _card_map [52] =
{
0x10000000000001,
0x10000000000002,
0x10000000000004,
0x10000000000008,
0x10000000000010,
0x10000000000020,
0x10000000000040,
0x10000000000080,
0x10000000000100,
0x10000000000200,
0x10000000000400,
0x10000000000800,
0x10000000001000,
0x80000000002000,
0x80000000004000,
0x80000000008000,
0x80000000010000,
0x80000000020000,
0x80000000040000,
0x80000000080000,
0x80000000100000,
0x80000000200000,
0x80000000400000,
0x80000000800000,
0x80000001000000,
0x80000002000000,
0x400000004000000,
0x400000008000000,
0x400000010000000,
0x400000020000000,
0x400000040000000,
0x400000080000000,
0x400000100000000,
0x400000200000000,
0x400000400000000,
0x400000800000000,
0x400001000000000,
0x400002000000000,
0x400004000000000,
0x2000008000000000,
0x2000010000000000,
0x2000020000000000,
0x2000040000000000,
0x2000080000000000,
0x2000100000000000,
0x2000200000000000,
0x2000400000000000,
0x2000800000000000,
0x2001000000000000,
0x2002000000000000,
0x2004000000000000,
0x2008000000000000
};

CardContainer :: CardContainer():_key(0) { }

void CardContainer :: addCard(Card card) {
	_key |= _card_map[card];
}

void CardContainer :: removeCard(Card card) {
	_key -= _card_map[card];
}

void CardContainer :: reset() {
	_key = 0; 
}

uint64_t CardContainer :: getKey() const {
	return _key;
}

int CardContainer :: getCards(int* array) const {
	uint64_t key = (_key & 0x000fffffffffffff);
	int counter = 0;
	while(key) {
		array[counter] = __builtin_ctzll(key);
		key -= (1ll << array[counter]);
		++counter;
	}
	return counter;
	
}

 void CardContainer :: addCard(Card* array, int size) {
	for(int i=0; i!= size; i++)
		this->addCard(array[i]);
}

void CardContainer :: removeCard(Card* array, int size) {
	for(int i=0; i != size; i++)
		this->removeCard(array[i]);
}

CardContainer :: CardContainer (Card* array, int size):_key(0) {
	for(int i=0; i!= size ; ++i)
		addCard(array[i]);
}