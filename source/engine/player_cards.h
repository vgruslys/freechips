#ifndef __PLAYER_CARDS__
#define __PLAYER_CARDS__
#include "card.h"


/*PlayerCards - an obvious class*/

class PlayerCards {
	public:
		void addCard(Card);
		void removeCard(Card);
		
	    uint64_t getKey() const;
		uint64_t getCodedKey() const;
		
		PlayerCards();
	private:
		uint64_t _key;
		uint64_t _coded_key;
		
		uint64_t* _card_map; //This points to a 52-entry array which converts a Card to a a uint64_t 
							// of the form 0000100
		uint64_t* _coded_card_map; //This points to a 52-entry array which converts a Card to a coded card
									//(which can then be added to form the coded community key)

};
#endif