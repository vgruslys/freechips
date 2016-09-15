#ifndef __COMMUNITY__
#define __COMMUNITY__
#include "card.h"
/*Community - a class for friends */

class Community {
    public:
		void addCard(Card); // Add a card to the community
		void removeCard(Card); //Removes the card Card BEWARE: you must NOT remove a card which isn't there already
		void reset(); //empties the community
		
		uint64_t getKey() const; //returns the normal key which is 1 bit per card, 52 bits in total
		uint64_t getCodedKey() const; //returns the additive key
		
		Community();
		~Community();
	private:
		uint64_t* _card_map; //This points to a 52-entry array which converts a Card to a a uint64_t 
							// of the form 0000100
		uint64_t* _coded_card_map; //This points to a 52-entry array which converts a Card to a coded card
									//(which can then be added to form the coded community key)
		
		uint64_t _key; //The standard 00000010000 64bit key
		uint64_t _coded_key; //The additive key
		

};
#endif
