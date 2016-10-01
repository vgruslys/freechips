#ifndef __COMMUNITY__
#define __COMMUNITY__
#include "card.h"
/*Community - a class for friends */

class Community {
    public:
		void addCard(Card); // Add a card to the community
		void removeCard(Card); //Removes the card Card BEWARE: you must NOT remove a card which isn't there already
		void reset(); //empties the community
		
		uint64_t getKey() const; //returns the normal key which is 1 bit per card, 52 bits in total, 
		                         //plus 4 x 3 bits for suit counts
		Community();

	private:
		static const uint64_t _card_map[52]; //This is a 52-entry array which converts a Card to a a uint64_t 
		uint64_t _key; //The key

};
#endif
